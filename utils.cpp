#include <random>
#include <cmath>
#include <tuple>
#include <vector>
#include "utils.h"


std::ostream& operator<<(std::ostream& os, const location& loc)
{
    os << "{" << loc.id << ", " << loc.x << ", " << loc.y << ", " 
       << loc.f << ", " << loc.b << ", " << loc.capacity << ", " 
       << loc.connected_to << "}";
    return os;
}

std::string generate_timestamped_filename(const std::string &folder, const std::string &base_name, const std::string &extension)
{
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    // Format the time as YYYY-MM-DD_HH-MM-SS
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&current_time), "%Y-%m-%d_%H-%M-%S");

    // Generate a 2-byte random hexadecimal code
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 0xFFFF); // Range for 2 bytes
    std::ostringstream hex_oss;
    hex_oss << std::hex << std::setw(4) << std::setfill('0') << dist(gen);

    // Construct the full path with folder, base name, timestamp, random hex code, and extension
    return folder + "/" + base_name + "_" + oss.str() + "_" + hex_oss.str() + extension;
}

// Function to save points to a file with comma as delimiter
void save_points_to_file(const std::vector<location> &points, const std::string &filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (const auto &point : points)
    {
        file << std::fixed << std::setprecision(4)
             << point.id << ","  // id
             << point.x << ","   // x-coordinate
             << point.y << ","   // y-coordinate
             << point.f << ","   // cost
             << point.b << ","   // num_clients
             << point.b_noisy << "\n";
    }
    file.close();
    std::cout << "Points saved to " << filename << std::endl;
}

// Function to save results to a file with comma as delimiter
void save_results_to_file(const std::vector<location> &points, const std::string &filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Error opening file: " + filename);
    }

    for (const auto &point : points)
    {
        file << std::fixed << std::setprecision(4)
             << point.id << ","             // id
             << point.x << ","              // x_u
             << point.y << ","              // y_u
             << point.capacity << ","       // c_u
             << point.connected_to << ","   // connected_to
             << point.delta << "\n";        // delta
    }
    file.close();
    std::cout << "Assignments saved to " << filename << std::endl;
}

// Function to load points from a file with comma as delimiter
std::vector<location> load_points_from_file(const std::string &filename)
{
    std::vector<location> points;
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream stream(line);
        std::string token;
        
        int id;
        double x, y, f, b, noisy_b;

        // Read values separated by commas
        std::getline(stream, token, ',');
        id = std::stoi(token);

        std::getline(stream, token, ',');
        x = std::stod(token);

        std::getline(stream, token, ',');
        y = std::stod(token);

        std::getline(stream, token, ',');
        f = std::stod(token);

        std::getline(stream, token, ',');
        b = std::stod(token);

        std::getline(stream, token, ',');
        noisy_b = std::stod(token);

        points.emplace_back(location{id, x, y, f, b, 0.0, -1, noisy_b});
    }
    file.close();
    return points;
}


// Function to calculate the Euclidean distance between two points
double euclidean_distance(double x1, double y1, double x2, double y2)
{
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// Function to generate Laplacian noise
double laplacian(double scale)
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_real_distribution<double> uniform_dist(0.0, 1.0);

    // Generate a random number in [0, 1]
    double u = uniform_dist(generator) - 0.5;

    // Apply the inverse Laplacian CDF
    return scale * (u < 0 ? log(1 + 2 * u) : -log(1 - 2 * u));
}

// Function to apply Laplacian noise
std::vector<location> apply_laplacian(const std::vector<location> &points, float eps)
{
    std::vector<location> noisy_points = points;

    // Scale parameter for the Laplacian noise
    double scale = 1.0 / eps;

    // Apply noise to each point
    for (auto &point : noisy_points)
    {
        // Generate Laplacian noise
        double noise = laplacian(scale);
        point.b_noisy = point.b + noise;
    }

    return noisy_points;
}

// Validates if every facility has the capacity to serve all of its connected clients
bool validate_solution(const std::vector<location> &points)
{
    std::vector<int> assigned_clients(points.size(), 0);

    for (const location &point : points)
    {
        // std::cout << point.id << " is connected to " << point.connected_to << " and has " << point.b << " clients." << std::endl;
        assigned_clients[point.connected_to] += point.b;
    }

    for (size_t i = 0; i < assigned_clients.size(); ++i)
    {
        if (points[i].capacity < assigned_clients[i])
        {
            std::cout << "Failure at location " << points[i].id << " capacity: " << points[i].capacity << " assigned clients: " << assigned_clients[i] << std::endl;
            return false;
        }
    }

    return true;
}

// Computes facility and connection costs for an assignment
std::tuple<double, double> compute_costs(const std::vector<location> &points){
    double facility_costs = 0;
    double connection_costs = 0;

    for (const location &point : points){
        facility_costs += point.capacity * point.f;
        connection_costs += point.b * euclidean_distance(point.x, point.y, points[point.connected_to].x, points[point.connected_to].y);
    }

    return std::tuple<double,double>{facility_costs, connection_costs};
}