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

#include <Eigen/Dense>

#include "distances.h"
#include "neighborhood.h"
#include "outliers.h"
#include "pbar.h"

namespace py = pybind11;

py::array_t<double> dbsod(
    py::array_t<double, py::array::c_style> data,
    std::string metric,
    std::vector<float> epsSpace,
    int minPts
) {
    // validate input
    if (data.size() == 0) {
        throw std::invalid_argument("`data` is empty.");
    }
    if (epsSpace.empty()) {
        throw std::invalid_argument("`epsSpace` is empty.");
    }

    // map data to Eigen
    auto buf = data.request();
    int rows = buf.shape[0];
    int cols = buf.shape[1];
    double* dataPtr = static_cast<double*>(buf.ptr);
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> X(dataPtr, rows, cols);

    // get distance function based on metricPtr argument
    std::function<double(const Eigen::VectorXd&, const Eigen::VectorXd&)> distanceFn;
    auto it = distanceFns.find(metric);
    if (it != distanceFns.end()) {
        distanceFn = it->second;
    } else {
        throw std::runtime_error(
            "`metric` must be one of [\"euclidean\", \"manhattan\", \"cosine\"]."
        );
    }

    // get neighbors within maxEps radius 
    float maxEps = *std::max_element(epsSpace.begin(), epsSpace.end());
    std::vector<std::vector<std::pair<int, float>>> neighbors = brute(X, distanceFn, maxEps);

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
