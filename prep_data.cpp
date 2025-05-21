#include "utils.h"
#include <random>

void load_real_world_locations(std::string filename, std::string out_filename, double f_min, double f_max, double eps){
    std::vector<location> instance;
    std::default_random_engine generator(std::random_device{}());
    std::uniform_real_distribution<double> uniform_cost(f_min, f_max);
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::string line;
    std::getline(file, line); // header

    // Variables to track min and max of x and y for scaling
    double x_min = std::numeric_limits<double>::max();
    double x_max = std::numeric_limits<double>::lowest();
    double y_min = std::numeric_limits<double>::max();
    double y_max = std::numeric_limits<double>::lowest();

    // First pass to determine min and max of x and y
    std::vector<std::tuple<int, double, double, double>> raw_data;
    while (std::getline(file, line))
    {
        std::istringstream stream(line);
        std::string token;
        
        int id;
        double x, y, b;

        std::getline(stream, token, ',');
        id = std::stoi(token) - 1; // start indexing at 0

        if (id >= 101){
            id--; // The id 101 is missing in the raw data. this is a quick fix
        }

        std::getline(stream, token, ',');
        x = std::stod(token);

        std::getline(stream, token, ',');
        y = std::stod(token);

        std::getline(stream, token, ',');
        b = std::stod(token);

        // Update min and max
        if (x < x_min) x_min = x;
        if (x > x_max) x_max = x;
        if (y < y_min) y_min = y;
        if (y > y_max) y_max = y;

        // Store raw data for second pass
        raw_data.emplace_back(id, x, y, b);
    }

    // Rewind the data for processing with scaling
    for (const auto& [id, x, y, b] : raw_data)
    {
        // Scale x and y
        double scaled_x = (x - x_min) / (x_max - x_min);
        double scaled_y = (y - y_min) / (y_max - y_min);

        // Generate facility cost
        double f = uniform_cost(generator);

        // Add scaled point to the instance
        instance.emplace_back(location{id, scaled_x, scaled_y, f, b, 0.0, -1, 0});
    }

    // Apply Laplacian noise to the instance
    std::vector<location> noisy_instance = apply_laplacian(instance, eps);

    // Save noisy points to file
    save_points_to_file(noisy_instance, out_filename);
}


int main(int argc, char* argv[]) {
    // Ensure the correct number of arguments are provided
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <filename> <out_filename> <f_min> <f_max> <eps>" << std::endl;
        return 1;
    }

    try {
        // Parse arguments
        std::string filename = argv[1];
        std::string out_filename = argv[2];
        double f_min = std::stod(argv[3]);
        double f_max = std::stod(argv[4]);
        double eps = std::stod(argv[5]);

        // Call prep_locations
        load_real_world_locations(filename, out_filename, f_min, f_max, eps);

        std::cout << "Locations prepared and saved successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}