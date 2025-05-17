#include "neighborhood.h"
#include <vector>
#include <functional>
#include <Eigen/Dense>

std::vector<std::vector<std::pair<int, float>>> brute(
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> X,
    std::function<double(const Eigen::VectorXd& a, const Eigen::VectorXd& b)> distanceFn,
    float distMax
) {
    const int N = X.rows();
    std::vector<std::vector<std::pair<int, float>>> neighbors(N);

    // iterate over full dataset computing pairwise distances between points
    for (int i = 0; i < N; ++i) {
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
