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

#ifndef DISTANCES_H
#define DISTANCES_H

#include <unordered_map>
#include <string>
#include <functional>
#include <Eigen/Dense>

double euclidean(const Eigen::VectorXd& a, const Eigen::VectorXd& b);
double manhattan(const Eigen::VectorXd& a, const Eigen::VectorXd& b);
double cosine(const Eigen::VectorXd& a, const Eigen::VectorXd& b);
extern const std::unordered_map<std::string, std::function<double(const Eigen::VectorXd&, const Eigen::VectorXd&)>> distanceFns;

#endif // DISTANCES_H
