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

constexpr double INF = std::numeric_limits<double>::max();

namespace py = pybind11;

namespace dbsod {

DBSOD::DBSOD(const std::span<const double> eps_space_, size_t min_pts)
    : eps_space(eps_space_.begin(), eps_space_.end()), min_pts(min_pts)
{
    // sort eps space
    std::sort(eps_space.begin(), eps_space.end());
}

void DBSOD::compute_core_threshold(const std::vector<std::vector<kd_tree::Neighbor>> &neighbors) {
    size_t n = neighbors.size();
    core_threshold.assign(n, INF);
    
    // core threshold is squared distance from a point to its min_pts-th neighbor
    // (0-indexed, because point is its own neighbor)
    for (size_t i = 0; i < n; ++i) {
        if (neighbors[i].size() > min_pts) {
            core_threshold[i] = neighbors[i][min_pts].dist2;
        }
    }
}

void DBSOD::compute_outlierness_score(const std::vector<std::vector<kd_tree::Neighbor>> &neighbors) {
    size_t n = neighbors.size();
    size_t m = eps_space.size();
    outlierness_score.assign(n, 0.0);

    std::vector<size_t> ptr(n, 0);
    std::vector<double> labels(n, 1.0);  // all points are initially outliers
    std::vector<bool> core(n, false); 

    size_t current = 0;
    for (double eps : eps_space) {
        pbar(/*current=*/current++, /*total=*/m - 1, /*width=*/20, /*desc=*/"Identifying outliers for each value in `eps_space`:");
        
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
            outlierness_score[i] += labels[i];
        }
    }
}

void normalize(std::vector<double> &vec) {
    double max_val = *std::max_element(vec.begin(), vec.end());
    if (max_val > 0.0) {
        for (double &val : vec) {
            val /= max_val;
        }
    }
}

DBSOD& DBSOD::fit(const std::span<const double> data, size_t rows, size_t cols) {
    // validate input
    if (data.size() == 0) {
        throw std::invalid_argument("`data` is empty.");
    }
    if (eps_space.empty()) {
        throw std::invalid_argument("`eps_space` is empty.");
    }

    // build k-d tree
    tree = std::make_unique<kd_tree::KDTree>(data, rows, cols);

    // get radius neighborhood graph
    max_eps = *std::max_element(eps_space.begin(), eps_space.end());
    auto neighbors = tree->radius_neighborhood_graph(max_eps);

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

    // compute core threshold and outlierness score for each point
    compute_core_threshold(neighbors);
    compute_outlierness_score(neighbors);

    // normalize outlierness scores
    normalize(outlierness_score);

    return *this;
}

std::vector<double> DBSOD::predict(const std::span<const double> data, size_t rows, size_t cols) {
    std::vector<double> result(rows, 0.0);

    // compute outlierness score for each point
    for (size_t i = 0; i < rows; ++i) {
        auto query = data.subspan(i * cols, cols);
        auto neighbors = tree->query_radius(query, max_eps);

        // sort neighbors by distance
        std::sort(
            neighbors.begin(),
            neighbors.end(),
            [](const auto &a, const auto &b) {
                return a.dist2 < b.dist2;
            }
        );

        double score = 0.0;
        bool is_outlier = true;

        for (double eps : eps_space) {
            double eps2 = eps * eps;

            size_t neighbor_idx = 0;
            while (
                is_outlier
                && neighbor_idx < neighbors.size()
                && neighbors[neighbor_idx].dist2 < eps2
            ) {
                // mark point as non-outlier as soon as there is a reachable neighbor
                // which is a core point from this epsilon value onward
                if (core_threshold[neighbors[neighbor_idx].index] <= eps2) {                   
                    is_outlier = false;
                }
                neighbor_idx++;
            }

            if (!is_outlier) break;  // end loop as the point is no longer an outlier
        
            score += 1.0;
        }

        result[i] = score;
    }

    // normalize outlierness scores
    normalize(result);

    return result;
}

std::vector<double> dbsod(
    const std::span<const double> data,
    size_t rows,
    size_t cols,
    const std::span<const double> eps_space,
    size_t min_pts
) {
    return DBSOD(eps_space, min_pts).fit(data, rows, cols).outlierness_score;
}

}  // namespace dbsod
