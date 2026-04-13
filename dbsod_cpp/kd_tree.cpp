#include "kd_tree.h"

#include <vector>
#include <span>
#include <stack>
#include <numeric>
#include <memory>
#include <algorithm>

#include "pbar.h"

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

std::vector<Neighbor> KDTree::query_radius(const std::span<const double> &query, double r) const {
    // validate query dimensionality
    if (query.size() != cols) {
        throw std::invalid_argument("Query dimensionality mismatch");
    }

    auto compute_dist2 = [&](size_t row) -> double {
        double sum = 0.0;
        for (size_t i = 0; i < cols; ++i) {
            double diff = query[i] - get_value(row, i);
            sum += diff * diff;
        }
        return sum;
    };

    std::vector<Neighbor> result;
    double r2 = r * r;

    std::stack<const TreeNode*> nodes_to_visit;
    nodes_to_visit.push(root.get());

    // traverse tree
    while (!nodes_to_visit.empty()) {
        const TreeNode *node = nodes_to_visit.top();
        nodes_to_visit.pop();

        if (!node) continue;

        double dist2 = compute_dist2(node->index);
        
        // check current point
        if (dist2 <= r2) result.emplace_back(node->index, dist2);

        size_t axis = node->axis;

        // define near and far branches
        double diff = query[axis] - get_value(node->index, axis);
        
        TreeNode *near;
        TreeNode *far;

        if (diff < 0) {
            near = node->left.get();
            far = node->right.get();
        } else {
            far = node->left.get();
            near = node->right.get();
        }

        // always explore near branch
        nodes_to_visit.push(near);

        // explore far branch if the sphere intersects it
        if (r2 >= diff * diff) nodes_to_visit.push(far);
    }

    return result;
}

std::vector<std::vector<Neighbor>> KDTree::radius_neighborhood_graph(double r) const {
    std::vector<std::vector<Neighbor>> result;
    result.resize(rows);

    // query for each row
    for (size_t i = 0; i < rows; ++i) {
        pbar(/*current=*/i, /*total=*/rows - 1, /*width=*/20, /*desc=*/"Building neighborhood graph:");

        auto query = data.subspan(i * cols, cols);
        result[i] = query_radius(query, r);
    }

    return result;
}

}  // namespace kd_tree
