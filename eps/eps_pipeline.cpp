#include "eps_pipeline_utils.h"

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        std::cerr << "Usage: " << argv[0] << " <filename> <eps_step> <eps_min> <eps_max> <alpha> <delta>" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::cout << "Loading points from: " << input_file << std::endl;

    double eps_step = std::stod(argv[2]);
    double eps_min = std::stod(argv[3]);
    double eps_max = std::stod(argv[4]);
    double alpha = std::stod(argv[5]);
    double delta = std::stod(argv[6]);

    // Output folder
    std::string output_folder = "out";
    if (!fs::exists(output_folder))
    {
        fs::create_directory(output_folder);
        std::cout << "Created folder: " << output_folder << std::endl;
    }

    // Load points
    auto instance = load_points_from_file(input_file);
    std::cout << "Loaded " << instance.size() << " points." << std::endl;

    eps_benchmark_result result = run_eps(instance, eps_step, eps_min, eps_max, alpha, delta);

    std::vector<eps_benchmark_result> results;
    results.push_back(result);

    save_eps_benchmark_results(results);

    return 0;
}