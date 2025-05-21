#include "./delta_pipeline_utils.h"

int main(int argc, char *argv[])
{
    // Check for the correct number of arguments
    if (argc < 13)
    {
        std::cerr << "Usage: " << argv[0] << " <instance_amount> <n> <width> <height> <delta> <delta_step> <eps> <alpha> <gamma> <f_min> <f_max> <use_uniform>" << std::endl;
        return 1;
    }

    // Parse arguments
    int instance_amount = std::atoi(argv[1]);
    int n = std::atoi(argv[2]);
    double width = std::atoi(argv[3]);
    double height = std::atof(argv[4]);
    double delta = std::atof(argv[5]);
    double delta_step = std::atof(argv[6]);
    double eps = std::atof(argv[7]);
    double alpha = std::atof(argv[8]);
    double gamma = std::atof(argv[9]);
    double f_min = std::atof(argv[10]);
    double f_max = std::atof(argv[11]);
    bool use_uniform = std::atoi(argv[12]) != 0;

    run(instance_amount, n, width, height, delta, delta_step, eps, alpha, gamma, f_min, f_max, use_uniform);
    return 0;
}