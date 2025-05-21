#include "pipeline_utils.h"

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 5)
        {
            throw std::runtime_error("Usage: ./pipeline <input_filename> <eps> <alpha> <delta>");
        }

        std::string input_file = argv[1];
        std::cout << "Loading points from: " << input_file << std::endl;

        double eps = std::stod(argv[2]);
        double alpha = std::stod(argv[3]);
        double delta = std::stod(argv[4]);

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

        auto result = pipeline(instance, input_file, eps, alpha, delta, true);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}