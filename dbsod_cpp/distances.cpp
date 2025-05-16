#include "distances.h"
#include <string>
#include <functional>
#include <unordered_map>
#include <Eigen/Dense>

// compute euclidean (L2) distance between two vectors
double euclidean(const Eigen::VectorXd& a, const Eigen::VectorXd& b) {
    return (a - b).norm();
}

// compute manhattan (L1) distance between two vectors
double manhattan(const Eigen::VectorXd& a, const Eigen::VectorXd& b) {
    return (a - b).cwiseAbs().sum();
}

// compute cosine distance between two vectors
double cosine(const Eigen::VectorXd& a, const Eigen::VectorXd& b) {
    return 1.0 - (a.dot(b) / (a.norm() * b.norm()));
}

const std::unordered_map<std::string, std::function<double(const Eigen::VectorXd&, const Eigen::VectorXd&)>> distanceFns = {
    {"euclidean", euclidean},
    {"manhattan", manhattan},
    {"cosine", cosine}
};
