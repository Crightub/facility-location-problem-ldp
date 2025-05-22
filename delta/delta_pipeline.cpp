#include "delta_pipeline_utils.h"

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        std::cerr << "Usage: " << argv[0] << " <filename> <eps> <alpha> <delta_step> <delta_max>" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::cout << "Loading points from: " << input_file << std::endl;

    double eps = std::stod(argv[2]);
    double alpha = std::stod(argv[3]);
    double delta_step = std::stod(argv[4]);
    double delta_max = std::stod(argv[5]);

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

    delta_benchmark_result result = run_delta(instance, eps, alpha, delta_step, delta_max);

    std::string base_name = "pipeline_" + std::to_string(delta_max) + "_" + std::to_string(delta_step);
    std::string filename = generate_timestamped_filename("delta/out", base_name, ".csv");
    save_results_to_file(result.best_reconn_out, filename);

    return 0;
}