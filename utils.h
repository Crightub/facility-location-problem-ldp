#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <cmath>
#include <set>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <limits>
#include <filesystem>

namespace fs = std::filesystem;

struct location
{
    int id;
    double x;
    double y;
    double f;
    double b;
    double capacity;
    int connected_to;
    double b_noisy;
    double delta;

    location(int id = -1, double x = 0.0, double y = 0.0, double f = 0.0, double b = 0.0, double capacity = 0.0, int connected_to = -1, double b_noisy = 0.0, double delta = 0.0)
        : id(id), x(x), y(y), f(f), b(b), capacity(capacity), connected_to(connected_to), b_noisy(b_noisy), delta(delta) {}
};

std::ostream& operator<<(std::ostream& os, const location& loc);

std::string generate_timestamped_filename(const std::string &folder, const std::string &base_name, const std::string &extension);

void save_points_to_file(const std::vector<location> &points, const std::string &filename);

void save_results_to_file(const std::vector<location> &assignments, const std::string &filename);

std::vector<location> load_points_from_file(const std::string &filename);

double euclidean_distance(double x1, double y1, double x2, double y2);

std::vector<location> apply_laplacian(const std::vector<location> &points, float eps);

bool validate_solution(const std::vector<location> &points);

std::tuple<double, double> compute_costs(const std::vector<location> &points);
#endif // UTILS_H
