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

#include <memory>
#include <vector>
#include <span>

namespace kd_tree {

struct Neighbor {
    size_t index;
    double dist2;
};

class KDTree {
  private:
    struct TreeNode {
        size_t index;
        size_t axis;
        std::unique_ptr<TreeNode> left;
        std::unique_ptr<TreeNode> right;
    };

    std::vector<double> data;  // row-major contiguous layout
    size_t rows, cols;
    std::unique_ptr<TreeNode> root;

    double get_value(size_t row, size_t col) const;

  public:
    KDTree(const std::span<const double> &data_, size_t rows_, size_t cols_);
    std::vector<Neighbor> query_radius(const std::span<const double> &query, double r) const;
    std::vector<std::vector<Neighbor>> radius_neighborhood_graph(double r) const;
};
    
}  // namespace kd_tree
