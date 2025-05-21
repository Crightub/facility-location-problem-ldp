#ifndef GENERATE_H
#define GENERATE_H

#include <vector>
#include "utils.h"

// Function declaration for generate_instance
// Generates an instance of the facility location problem
std::vector<location> generate_instance(
    double width,
    double height,
    double lambda_parent,
    double lambda_daughter,
    double delta,
    double f_min,
    double f_max,
    double b_mean,
    double b_std,
    int b_max);

std::vector<location> generate_instance_uniform(double width,
    double height,
    double lambda,
    double f_min,
    double f_max,
    double b_mean,
    double b_std,
    int b_max);

#endif // GENERATE_H
