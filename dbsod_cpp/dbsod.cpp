#include "dbsod.h"

#include <iostream>
#include <string>
#include <functional>

#include <Eigen/Dense>

#include "distances.h"
#include "neighborhood.h"

void dbsod(
    double* dataPtr,
    int rows,
    int cols,
    const char* metricPtr,
    float* epsSpacePtr,
    int numEpsValues
) {
    // validate input
    if (!dataPtr) {
        throw std::invalid_argument("Input data pointer is null.");
    }
    if (!metricPtr) {
        throw std::invalid_argument("Input metric pointer is null.");
    }
    if (!epsSpacePtr) {
        throw std::invalid_argument("Input epsSpace pointer is null.");
    }

    // form appropriate data structures from given pointers 
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> X(dataPtr, rows, cols);
    std::string metric(metricPtr);
    std::vector<float> epsSpace(epsSpacePtr, epsSpacePtr + numEpsValues);

    // get distance function based on metricPtr argument
    std::function<double(const Eigen::VectorXd& a, const Eigen::VectorXd& b)> distanceFn;
    auto it = distanceFns.find(metric);
    if (it != distanceFns.end()) {
        distanceFn = it->second;
    } else {
        throw std::runtime_error(
            "\"metricPtr\" argument must point to one of [\"euclidean\", \"manhattan\", \"cosine\"]."
        );
    }

    std::cout << "Hello from DBSOD!\n";
    std::cout << X << std::endl;
}
