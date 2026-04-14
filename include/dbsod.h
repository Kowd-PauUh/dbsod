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

#pragma once

#include <vector>
#include <span>
#include <memory>

#include "kd_tree.h"

namespace dbsod {

class DBSOD {
  private:
    std::vector<double> eps_space;
    size_t min_pts;

    // initialized in .fit
    std::unique_ptr<kd_tree::KDTree> tree;
    std::vector<double> core_threshold;

    void compute_core_threshold(const std::vector<std::vector<kd_tree::Neighbor>> &neighbors);
    void compute_outlierness_score(const std::vector<std::vector<kd_tree::Neighbor>> &neighbors);

  public:
    std::vector<double> outlierness_score;  // initialized in .fit

    DBSOD(const std::span<const double> eps_space, size_t min_pts);
    DBSOD& fit(const std::span<const double> data, size_t rows, size_t cols);
    std::vector<double> predict(const std::span<const double> data, size_t rows, size_t cols);
};

std::vector<double> dbsod(
    const std::span<const double> data,
    size_t rows,
    size_t cols,
    const std::span<const double> eps_space,
    size_t min_pts
);

}  // namespace dbsod
