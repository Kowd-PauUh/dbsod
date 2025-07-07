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

#include "neighborhood.h"
#include <vector>
#include <functional>
#include <Eigen/Dense>
#include "pbar.h"

std::vector<std::vector<std::pair<int, float>>> brute(
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> X,
    std::function<double(const Eigen::VectorXd& a, const Eigen::VectorXd& b)> distanceFn,
    float distMax
) {
    const int N = X.rows();
    std::vector<std::vector<std::pair<int, float>>> neighbors(N);

    // iterate over full dataset computing pairwise distances between points
    for (int i = 0; i < N; ++i) {
        pbar(/*current=*/i, /*total=*/N-1, /*width=*/20, /*desc=*/"Computing pairwise distances:                 ");

        Eigen::VectorXd xi = X.row(i);

        for (int j = i + 1; j < N; ++j) {
            Eigen::VectorXd xj = X.row(j);
            float d = static_cast<float>(distanceFn(xi, xj));

            // append if point is within specified radius
            if (d <= distMax) {
                neighbors[i].emplace_back(j, d);
                neighbors[j].emplace_back(i, d);
            }
        }
    }

    return neighbors;
}
