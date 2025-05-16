#ifndef DISTANCES_H
#define DISTANCES_H

#include <unordered_map>
#include <string>
#include <functional>
#include <Eigen/Dense>

double euclidean(const Eigen::VectorXd& a, const Eigen::VectorXd& b);
double manhattan(const Eigen::VectorXd& a, const Eigen::VectorXd& b);
double cosine(const Eigen::VectorXd& a, const Eigen::VectorXd& b);
extern const std::unordered_map<std::string, std::function<double(const Eigen::VectorXd&, const Eigen::VectorXd&)>> distanceFns;

#endif // DISTANCES_H
