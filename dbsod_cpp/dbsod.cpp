#include "dbsod.h"
#include <iostream>
#include <string>
#include <functional>
#include <Eigen/Dense>
#include "distances.h"

void dbsod(double* data, int rows, int cols, const char* metric) {
    // validate input
    if (!data) {
        throw std::invalid_argument("Input data pointer is null.");
    }

    // form appropriate data structures from given pointers 
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> X(data, rows, cols);
    std::string metricStr(metric);

    // get distance function based on metric argument
    std::function<double(const Eigen::VectorXd& a, const Eigen::VectorXd& b)> distanceFn;
    auto it = distanceFns.find(metricStr);
    if (it != distanceFns.end()) {
        distanceFn = it->second;
    } else {
        throw std::runtime_error(
            "\"metric\" argument must be one of [\"euclidean\", \"manhattan\", \"cosine\"]."
        );
    }

    std::cout << "Hello from DBSOD!\n";
    std::cout << X << std::endl;
}
