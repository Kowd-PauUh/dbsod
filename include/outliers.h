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
