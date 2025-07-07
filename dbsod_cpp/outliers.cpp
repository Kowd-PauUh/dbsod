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

#include "outliers.h"
#include <vector>
#include <Eigen/Dense>

Eigen::VectorXi outliers(
    std::vector<std::vector<std::pair<int, float>>>& neighbors,
    int minPts,
    float eps
) {
    const int N = neighbors.size();
    Eigen::VectorXi labels = Eigen::VectorXi::Constant(N, 1);  // 1 means outlier/noise
    std::vector<bool> core(N, false);

    // find core points
    for (int i = 0; i < N; i++) {
        // count neighbors
        int neighbors_cnt = 0;
        int max_j = neighbors[i].size();
        for (int j = 0; j < max_j; j++) {
            if (neighbors[i][j].second <= eps) {
                neighbors_cnt += 1;
            }
        }

        if (neighbors_cnt >= minPts) {
            core[i] = true;
        }
    }

    // expand clusters
    for (int i = 0; i < N; i++) {
        if (!core[i]) continue;  // skip non-core points

        labels[i] = 0;  // not an outlier/noise
        int max_j = neighbors[i].size();
        for (int j = 0; j < max_j; j++) {
            if (neighbors[i][j].second <= eps) {
                int index = neighbors[i][j].first;
                labels[index] = 0;  // not an outlier/noise
            }
        }
    }

    return labels;
}
