#include "utils.h"
#include "delta_pipeline_utils.h"
#include <random>

std::vector<location> load_real_world_locations(std::string filename, double f_min, double f_max){
    std::vector<location> instance;
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

        // Add scaled point to the instance
        instance.emplace_back(location{id, scaled_x, scaled_y, 0, b, 0.0, -1, 0});
    }

    return instance;
}

/*
    Add random facility costs to instance and apply laplacian noise
*/
std::vector<location> prep_instance(std::vector<location> instance,  double eps, double f_min, double f_max){
    std::default_random_engine generator(std::random_device{}());
    std::uniform_real_distribution<double> uniform_cost(f_min, f_max);

    for (location& point : instance){
        point.f = uniform_cost(generator);
    }

    std::vector<location> noisy_instance = apply_laplacian(instance, eps);
    return noisy_instance;
}

void run(std::string filename, int instance_amount, double eps, double alpha, double f_min, double f_max){
    std::vector<location> instance = load_real_world_locations(filename, f_min, f_max);

    std::vector<delta_benchmark_result> results = std::vector<delta_benchmark_result>(instance_amount);
    for (int i = 0; i < instance_amount; ++i){
        std::vector<location> noisy_instance = prep_instance(instance, eps, f_min, f_max);
        delta_benchmark_result result = run_delta(noisy_instance, eps, alpha, 0.01, 1);   
        results[i] = result;
    }

    std::string base_name = filename + "_out_" + std::to_string(f_min) + "_" + std::to_string(f_max);
    std::string out_filename = generate_timestamped_filename("real_world/out", base_name, ".csv");
    save_delta_benchmark_results(results, filename);
}

int main(int argc, char *argv[])
{
    // Check for the correct number of arguments
    if (argc < 7)
    {
        std::cerr << "Usage: " << argv[0] << " <filename> <instance_amount> <eps> <alpha> <f_min> <f_max>" << std::endl;
        return 1;
    }

    // Parse arguments
    std::string filename = argv[1];
    int instance_amount = std::atoi(argv[2]);
    double eps = std::atof(argv[3]);
    double alpha = std::atof(argv[4]);
    double f_min = std::atof(argv[5]);
    double f_max = std::atof(argv[6]);

    run(filename, instance_amount, eps, alpha, f_min, f_max);

    return 0;
}