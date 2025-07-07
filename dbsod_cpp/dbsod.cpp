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

#include <iostream>
#include <string>
#include <functional>
#include <algorithm>

#include <Eigen/Dense>

#include "distances.h"
#include "neighborhood.h"
#include "outliers.h"
#include "pbar.h"

double* dbsod(
    double* dataPtr,
    int rows,
    int cols,
    const char* metricPtr,
    float* epsSpacePtr,
    int numEpsValues,
    int minPts
) {
    // validate input
    if (!dataPtr) {
        throw std::invalid_argument("Input data pointer is null.");
    }
    if (!metricPtr) {
        throw std::invalid_argument("Input metric pointer is null.");
    }
    if (!epsSpacePtr) {
        throw std::invalid_argument("Input epsSpace pointer is null.");
    }

    // form appropriate data structures from given pointers 
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> X(dataPtr, rows, cols);
    std::string metric(metricPtr);
    std::vector<float> epsSpace(epsSpacePtr, epsSpacePtr + numEpsValues);
    if (epsSpace.empty()) {throw std::invalid_argument("Empty epsSpace.");}

    // get distance function based on metricPtr argument
    std::function<double(const Eigen::VectorXd& a, const Eigen::VectorXd& b)> distanceFn;
    auto it = distanceFns.find(metric);
    if (it != distanceFns.end()) {
        distanceFn = it->second;
    } else {
        throw std::runtime_error(
            "\"metricPtr\" argument must point to one of [\"euclidean\", \"manhattan\", \"cosine\"]."
        );
    }

    // get neighbors within maxEps radius 
    float maxEps = *std::max_element(epsSpace.begin(), epsSpace.end());
    std::vector<std::vector<std::pair<int, float>>> neighbors = brute(X, distanceFn, maxEps);

    // compute outlierness scores
    Eigen::VectorXi scores = Eigen::VectorXi::Zero(rows);
    for (int i = 0; i < numEpsValues; i++) {
        pbar(/*current=*/i, /*total=*/numEpsValues-1, /*width=*/20, /*desc=*/"Identifying outliers for each `epsilon` value:");

        float eps = epsSpace[i];
        Eigen::VectorXi result = outliers(neighbors, minPts, eps);
        scores += result;
    }

    // normalize outlierness scores
    Eigen::VectorXd normalizedScores = scores.cast<double>() / epsSpace.size();

    // allocate memory to return
    double* result = new double[rows];
    std::copy(normalizedScores.begin(), normalizedScores.end(), result);

    return result;
}

void free_array(double* arrayPtr) {
    delete[] arrayPtr;
}
