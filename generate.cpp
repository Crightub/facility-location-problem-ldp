#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <filesystem>
#include <cstdlib>
#include <math.h>
#include "utils.h"

namespace fs = std::filesystem;

// Generates locations according to poisson point process
// Locations only contain x and y coordinates
// facility costs and client information are filled in seperately
std::vector<location> poisson_point_process(double lambda, double width, double height)
{
    std::vector<location> points;
    std::default_random_engine generator(std::random_device{}());
    std::poisson_distribution<int> poisson(lambda * width * height);
    std::uniform_real_distribution<double> uniform_x(0.0, width);
    std::uniform_real_distribution<double> uniform_y(0.0, height);

    int num_points = poisson(generator);

    for (int i = 0; i < num_points; ++i)
    {
        double x = uniform_x(generator);
        double y = uniform_y(generator);
        points.emplace_back(location{i, x, y});
    }

    return points;
}

std::vector<location> matern_point_process(double lambda_parent, double lambda_daughter, double delta, double width, double height)
{
    std::default_random_engine generator(std::random_device{}());
    std::poisson_distribution<int> poisson_parent(lambda_parent);
    std::uniform_real_distribution<double> uniform_x_parent(delta, width+delta);
    std::uniform_real_distribution<double> uniform_y_parent(delta, height+delta);
    std::poisson_distribution<int> poisson_daughter(lambda_daughter);

    int num_centers = poisson_parent(generator);
    
    std::vector<location> centers = std::vector<location>(num_centers);

    for (int i = 0; i < num_centers; ++i)
    {
        double x = uniform_x_parent(generator);
        double y = uniform_y_parent(generator);
        centers[i] = location{i, x, y, 0.0, 0.0, 0.0, -1};
    }  

    std::vector<location> points;
    std::uniform_real_distribution<double> uniform_theta(0.0, M_PI * 2);
    std::uniform_real_distribution<double> uniform_rho(0.0, 1);

    int id = 0;
    for (int i = 0; i < centers.size(); ++i)
    {
        location center = centers.at(i);
        int num_daughter = poisson_daughter(generator);

        for (int j = 0; j < num_daughter; ++j)
        {
            double theta = uniform_theta(generator);
            double rho = delta * std::sqrt(uniform_rho(generator));

            // Convert to cartesian coordinates
            double x = center.x + rho * cos(theta);
            double y = center.y + rho * sin(theta);

            points.emplace_back(location{id, x, y});

            ++id;
        }
    }

    return points;
}

// Generates instance of the facility location problem
// locations V: amount distribited according to possion distribution
//              individual points are then distributed uniformly at random
// facility costs f: uniformly at random in interval [f_min, f_max]
// clients b: gaussian distribution rounded to nearest integer and clipped between [0, b_max]
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
    int b_max)
{
    // Random number generators
    std::default_random_engine generator(std::random_device{}());
    std::uniform_real_distribution<double> uniform_cost(f_min, f_max);
    std::normal_distribution<double> gaussian_clients(b_mean, b_std);

    std::vector<location> points = matern_point_process(lambda_parent, lambda_daughter, delta, width, height);

    for (location &point : points)
    {
        point.f = uniform_cost(generator);

        // Clamp the number of clients to [0, 8]
        int b = std::round(gaussian_clients(generator));
        point.b = std::max(0, std::min(b_max, b));
    }

    return points;
}


// Generates instance of the facility location problem
// locations V: amount distribited according to possion distribution
//              individual points are then distributed uniformly at random
// facility costs f: uniformly at random in interval [f_min, f_max]
// clients b: gaussian distribution rounded to nearest integer and clipped between [0, b_max]
std::vector<location> generate_instance_uniform(
    double width,
    double height,
    double lambda,
    double f_min,
    double f_max,
    double b_mean,
    double b_std,
    int b_max)
{
    // Random number generators
    std::default_random_engine generator(std::random_device{}());
    std::uniform_real_distribution<double> uniform_cost(f_min, f_max);
    std::normal_distribution<double> gaussian_clients(b_mean, b_std);

    std::vector<location> points = poisson_point_process(lambda, width, height);

    for (location &point : points)
    {
        point.f = uniform_cost(generator);

        // Clamp the number of clients to [0, 8]
        int b = std::round(gaussian_clients(generator));
        point.b = std::max(0, std::min(b_max, b));
    }

    return points;
}

// int main(int argc, char *argv[])
// {
//     // Check for correct number of arguments
//     if (argc != 11)
//     {
//         std::cerr << "Usage: " << argv[0] << " <width> <height> <lambda_parent> <lambda_daughter> <delta> <f_min> <f_max> <b_mean> <b_std> <b_max>\n";
//         return 1;
//     }

//     // Parse input parameters
//     double width = std::stod(argv[1]);
//     double height = std::stod(argv[2]);
//     double lambda_parent = std::stod(argv[3]);
//     double lambda_daughter = std::stod(argv[4]);
//     double delta = std::stod(argv[5]);
//     double f_min = std::stod(argv[6]);
//     double f_max = std::stod(argv[7]);
//     double b_mean = std::stod(argv[8]);
//     double b_std = std::stod(argv[9]);
//     double b_max = std::stod(argv[10]);

//     // Validate inputs
//     if (f_min < 0.0 || f_min > f_max)
//     {
//         std::cerr << "Error: cost_min > cost_max.\n";
//         return 1;
//     }

//     if (width <= 0.0 || height <= 0.0)
//     {
//         std::cerr << "Error: width and height must be positive values.\n";
//         return 1;
//     }

//     // Generate points
//     auto instance = generate_instance(width, height, lambda_parent, lambda_daughter, delta, f_min, f_max, b_mean, b_std, b_max);

//     // Ensure the "data" folder exists
//     std::string folder = "input";
//     if (!fs::exists(folder))
//     {
//         fs::create_directory(folder);
//         std::cout << "Created folder: " << folder << std::endl;
//     }

//     // Generate timestamped filename
//     std::string filename = generate_timestamped_filename(folder, "input", ".in");

//     // Save points to a file
//     save_points_to_file(instance, filename);

//     std::cout << "Points saved to file: " << filename << std::endl;

//     return 0;
// }
