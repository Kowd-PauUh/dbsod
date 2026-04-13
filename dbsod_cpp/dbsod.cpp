/*
 * Copyright 2025 Ivan Danylenko
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
#include <string>
#include <functional>
#include <algorithm>
#include <cmath>

#include <Eigen/Dense>

#include "kd_tree.h"
#include "outliers.h"
#include "pbar.h"

namespace py = pybind11;

py::array_t<double> dbsod(
    py::array_t<double, py::array::c_style> data,
    std::vector<double> epsSpace,
    int minPts
) {
    // validate input
    if (data.size() == 0) {
        throw std::invalid_argument("`data` is empty.");
    }
    if (epsSpace.empty()) {
        throw std::invalid_argument("`epsSpace` is empty.");
    }

    // get read-only data span
    auto buf = data.request();
    size_t rows = buf.shape[0];
    size_t cols = buf.shape[1];
    double* dataPtr = static_cast<double*>(buf.ptr);
    std::span<const double> span_data(dataPtr, rows * cols);

    // build k-d tree
    kd_tree::KDTree tree(span_data, rows, cols);

    // get neighbors within maxEps radius 
    double maxEps = *std::max_element(epsSpace.begin(), epsSpace.end());

    std::vector<std::vector<std::pair<int, float>>> neighbors;
    neighbors.resize(rows);

    std::vector<double> query(cols);

    for (size_t i = 0; i < rows; ++i) {
        // extract i-th row into query vector
        for (size_t j = 0; j < cols; ++j) {
            query[j] = dataPtr[i * cols + j];
        }

        auto result = tree.query_radius(query, maxEps);

        auto& out = neighbors[i];
        out.reserve(result.size());

        for (const auto& n : result) {
            out.emplace_back(static_cast<int>(n.index), static_cast<float>(std::sqrt(n.dist2)));
        }
    }

    // compute outlierness scores
    Eigen::VectorXi scores = Eigen::VectorXi::Zero(rows);
    for (size_t i = 0; i < epsSpace.size(); i++) {
        pbar(/*current=*/i, /*total=*/epsSpace.size()-1, /*width=*/20, /*desc=*/"Identifying outliers for each `epsilon` value:");
        float eps = epsSpace[i];
        Eigen::VectorXi result = outliers(neighbors, minPts, eps);
        scores += result;
    }

    // normalize outlierness scores
    Eigen::VectorXd normalizedScores = scores.cast<double>() / epsSpace.size();

    // return as NumPy array
    return py::array_t<double>(normalizedScores.size(), normalizedScores.data());
}
