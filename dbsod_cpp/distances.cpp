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

#include "distances.h"
#include <string>
#include <functional>
#include <unordered_map>
#include <Eigen/Dense>

// compute euclidean (L2) distance between two vectors
double euclidean(const Eigen::VectorXd& a, const Eigen::VectorXd& b) {
    return (a - b).norm();
}

// compute manhattan (L1) distance between two vectors
double manhattan(const Eigen::VectorXd& a, const Eigen::VectorXd& b) {
    return (a - b).cwiseAbs().sum();
}

// compute cosine distance between two vectors
double cosine(const Eigen::VectorXd& a, const Eigen::VectorXd& b) {
    return 1.0 - (a.dot(b) / (a.norm() * b.norm()));
}

const std::unordered_map<std::string, std::function<double(const Eigen::VectorXd&, const Eigen::VectorXd&)>> distanceFns = {
    {"euclidean", euclidean},
    {"manhattan", manhattan},
    {"cosine", cosine}
};
