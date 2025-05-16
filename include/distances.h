#ifndef DISTANCES_H
#define DISTANCES_H

#include <Eigen/Dense>

double euclidean(const Eigen::VectorXd& a, const Eigen::VectorXd& b);
double manhattan(const Eigen::VectorXd& a, const Eigen::VectorXd& b);
double cosine(const Eigen::VectorXd& a, const Eigen::VectorXd& b);

#endif // DISTANCES_H
