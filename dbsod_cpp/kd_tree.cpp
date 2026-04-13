#include "kd_tree.h"

#include <vector>
#include <span>
#include <numeric>
#include <memory>
#include <algorithm>


namespace kd_tree {

double KDTree::get_value(size_t row, size_t col) const {
    return data[row * cols + col];
}

KDTree::KDTree(const std::span<const double> &data_, size_t rows_, size_t cols_) : data(data_), rows(rows_), cols(cols_) {
    std::vector<size_t> indices(rows);
    std::iota(indices.begin(), indices.end(), 0);

    auto dfs = [&](auto self, size_t l, size_t r, size_t depth) -> std::unique_ptr<TreeNode> {
        if (r <= l) return nullptr;

        size_t axis = depth % cols;
        size_t mid = (l + r) / 2;

        // partition by median
        std::nth_element(
            indices.begin() + l,
            indices.begin() + mid,
            indices.begin() + r,
            [&](size_t a, size_t b) {
                return get_value(a, axis) < get_value(b, axis);
            }
        );

        // create node
        auto node = std::make_unique<TreeNode>();
        node->index = indices[mid];
        node->axis = axis;

        // recursively create children
        node->left = self(self, l, mid, depth + 1);
        node->right = self(self, mid + 1, r, depth + 1);

        return node;
    };

    root = dfs(dfs, 0, rows, 0);
}

}