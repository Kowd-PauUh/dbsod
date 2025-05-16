#include "distances.h"
#include <Eigen/Dense>

double euclidean(const Eigen::VectorXd& a, const Eigen::VectorXd& b) {
    return (a - b).norm();
}

double manhattan(const Eigen::VectorXd& a, const Eigen::VectorXd& b) {
    return (a - b).cwiseAbs().sum();
}

double cosine(const Eigen::VectorXd& a, const Eigen::VectorXd& b) {
    return 1.0 - (a.dot(b) / (a.norm() * b.norm()));
}
