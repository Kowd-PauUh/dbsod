#include "dbsod.h"

#include <iostream>
#include <string>
#include <functional>
#include <algorithm>

#include <Eigen/Dense>

#include "distances.h"
#include "neighborhood.h"
#include "outliers.h"

double* dbsod(
    double* dataPtr,
    int rows,
    int cols,
    const char* metricPtr,
    float* epsSpacePtr,
    int numEpsValues,
    int minPts
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
    if (epsSpace.empty()) {throw std::invalid_argument("Empty epsSpace.");}

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

    // get neighbors within maxEps radius 
    float maxEps = *std::max_element(epsSpace.begin(), epsSpace.end());
    std::vector<std::vector<std::pair<int, float>>> neighbors = brute(X, distanceFn, maxEps);

    // compute outlierness scores
    Eigen::VectorXi scores = Eigen::VectorXi::Zero(rows);
    for (float eps : epsSpace) {
        Eigen::VectorXi result = outliers(neighbors, minPts, eps);
        scores += result;
    }

    // normalize outlierness scores
    Eigen::VectorXd normalizedScores = scores.cast<double>() / epsSpace.size();

    // allocate memory to return
    double* result = new double[rows];
    std::copy(normalizedScores.begin(), normalizedScores.end(), result);

    return result;
}

void free_array(double* arrayPtr) {
    delete[] arrayPtr;
}
