#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

#include "neighborhood.h"
#include <vector>
#include <functional>
#include <Eigen/Dense>

std::vector<std::vector<std::pair<int, float>>> brute(
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> X,
    std::function<double(const Eigen::VectorXd& a, const Eigen::VectorXd& b)> distanceFn,
    float distMax
);

#endif // NEIGHBORHOOD_H
