#include "utils.h"

namespace fs = std::filesystem;

// Function to compute the assignments
std::vector<location> compute_assignments(const std::vector<location> &points)
{
    std::vector<location> assignment = points;
    std::vector<int> capacities(points.size(), 0);

    double total_facility_costs = 0;
    double total_connection_costs = 0;

    for (location& u : assignment)
    {
        double min_value = std::numeric_limits<double>::max();

        location* best_location;
        for (location& v : assignment)
        {
            double distance = euclidean_distance(u.x, u.y, v.x, v.y);
            double value = v.f + distance;
            if (value < min_value)
            {
                best_location = &v;
                min_value = value;
            }
        }
        if (!best_location){
            continue;
        }

        best_location->capacity += u.b;
        u.connected_to = best_location->id;

        total_connection_costs += u.b * euclidean_distance(u.x, u.y, best_location->x, best_location->y);
        total_facility_costs += u.b * best_location->f;
    }

    // std::cout 
    // << "Optimal solution:\n" << "Facility Costs: " 
    // << total_facility_costs 
    // << "\nConnection Costs: " 
    // << total_connection_costs 
    // << "\nTotal Costs: " 
    // << (total_connection_costs + total_facility_costs) 
    // << std::endl;  

    return assignment;
}

// int main(int argc, char *argv[])
// {
//     try
//     {
//         if (argc != 2)
//         {
//             throw std::runtime_error("Usage: ./opt <input_filename>");
//         }

//         std::string input_file = argv[1];
//         std::cout << "Loading points from: " << input_file << std::endl;

//         // Output folder
//         std::string output_folder = "out";
//         if (!fs::exists(output_folder))
//         {
//             fs::create_directory(output_folder);
//             std::cout << "Created folder: " << output_folder << std::endl;
//         }

//         // Load points
//         auto points = load_points_from_file(input_file);
//         std::cout << "Loaded " << points.size() << " points." << std::endl;

//         // Compute assignments
//         auto assignments = compute_assignments(points);

//         // Ensure the "data" folder exists
//         std::string folder = "out";
//         if (!fs::exists(folder))
//         {
//             fs::create_directory(folder);
//             std::cout << "Created folder: " << folder << std::endl;
//         }

//         // Generate timestamped filename
//         std::string filename = generate_timestamped_filename(folder, "out", ".dat");

//         // Save results
//         save_results_to_file(assignments, filename);
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return 1;
//     }

//     return 0;
// }
