/*
 * Copyright 2026 Ivan Danylenko
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dbsod.h"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <vector>
#include <functional>
#include <algorithm>
#include <limits>

#include "kd_tree.h"
#include "pbar.h"

constexpr double DBL_MAX = std::numeric_limits<double>::max();

namespace py = pybind11;

std::vector<double> get_core_threshold(
    const std::vector<std::vector<kd_tree::Neighbor>> &neighbors,
    size_t min_pts
) {
    size_t n = neighbors.size();
    std::vector<double> core_threshold(n, DBL_MAX);
    
    // core threshold is squared distance from a point to its min_pts-th neighbor
    // (0-indexed, because point is its own neighbor)
    for (size_t i = 0; i < n; ++i) {
        if (neighbors[i].size() > min_pts) {
            core_threshold[i] = neighbors[i][min_pts].dist2;
        }
    }

    return core_threshold;
}

std::vector<double> get_outlierness_score(
    const std::vector<std::vector<kd_tree::Neighbor>> &neighbors,
    std::vector<double> &eps_space,
    std::vector<double> &core_threshold
) {
    size_t n = neighbors.size();
    size_t m = eps_space.size();
    std::vector<double> result(n, 0.0);

    std::vector<size_t> ptr(n, 0);
    std::vector<double> labels(n, 1.0);  // all points are initially outliers
    std::vector<bool> core(n, false); 

    size_t current = 0;
    for (double eps : eps_space) {
        pbar(/*current=*/current++, /*total=*/m - 1, /*width=*/20, /*desc=*/"Identifying outliers for each `epsilon` value:");
        
        double eps2 = eps * eps;

        // identify which points are core for this epsilon value
        for (size_t i = 0; i < n; ++i) {
            core[i] = core_threshold[i] <= eps2;
        }

        // expand clusters
        for (size_t i = 0; i < n; ++i) {
            if (core[i]) {
                labels[i] = 0.0;

                // incremental neighbor expansion
                while (ptr[i] < neighbors[i].size() && neighbors[i][ptr[i]].dist2 <= eps2) {
                    labels[neighbors[i][ptr[i]].index] = 0.0;
                    ptr[i]++;
                }
            }
        }

        // aggregate labels
        for (size_t i = 0; i < n; ++i) {
            result[i] += labels[i];
        }
    }

    return result;
}

std::vector<double> dbsod(
    py::array_t<double, py::array::c_style> data,
    std::vector<double> eps_space,
    size_t min_pts
) {
    // validate input
    if (data.size() == 0) {
        throw std::invalid_argument("`data` is empty.");
    }
    if (eps_space.empty()) {
        throw std::invalid_argument("`eps_space` is empty.");
    }

    // get read-only data span
    auto buf = data.request();
    size_t rows = buf.shape[0];
    size_t cols = buf.shape[1];
    double* data_ptr = static_cast<double*>(buf.ptr);
    std::span<const double> span_data(data_ptr, rows * cols);

    // build k-d tree
    kd_tree::KDTree tree(span_data, rows, cols);

    // get radius neighborhood graph
    auto max_eps = *std::max_element(eps_space.begin(), eps_space.end());
    auto neighbors = tree.radius_neighborhood_graph(max_eps);

    // sort each point's neighbors by distance
    for (auto &n : neighbors) {
        std::sort(
            n.begin(),
            n.end(),
            [](const auto &a, const auto &b) {
                return a.dist2 < b.dist2;
            }
        );
    }

    // compute core threshold for each point
    auto core_threshold = get_core_threshold(neighbors, min_pts);

    // compute outlierness score for each point
    auto result = get_outlierness_score(neighbors, eps_space, core_threshold);

    return result;
}
