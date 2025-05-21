#ifndef PRIVACY_H
#define PRIVACY_H

#include <vector>
#include "utils.h"

std::vector<location> private_reconnection_assignment(const std::vector<location> &points, double eps, double alpha, double delta);
std::vector<location> private_assignment(const std::vector<location> &points, float eps, float alpha);
std::vector<location> reconnect(std::vector<location> points, double delta, std::unordered_map<int, std::unordered_set<int>> reverse_assignment);
#endif 
