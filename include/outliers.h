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

#ifndef OUTLIERS_H
#define OUTLIERS_H

#include "outliers.h"
#include <vector>
#include <Eigen/Dense>

Eigen::VectorXi outliers(
    std::vector<std::vector<std::pair<int, float>>>& neighbors,
    int minPts,
    float eps
);

#endif // OUTLIERS_H
