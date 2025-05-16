#include "dbsod.h"
#include <iostream>
#include <string>
#include <Eigen/Dense>
#include "distances.h"

void dbsod(double* data, int rows, int cols, std::string metric) {
    Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> X(data, rows, cols);

    std::cout << "Hello from DBSOD!\n";
    std::cout << X << std::endl;
}
