#include "eps_pipeline_utils.h"

int main(int argc, char *argv[])
{
    // Check for the correct number of arguments
    if (argc < 13)
    {
        std::cerr << "Usage: " << argv[0] << " <instance_amount> <n> <width> <height> <eps_step> <eps_min> <eps_max> <alpha> <delta> <gamma> <f_min> <f_max> <use_uniform>" << std::endl;
        return 1;
    }

    // Parse arguments
    int instance_amount = std::atoi(argv[1]);
    int n = std::atoi(argv[2]);
    double width = std::atoi(argv[3]);
    double height = std::atof(argv[4]);
    double eps_step = std::atof(argv[5]);
    double eps_min = std::atof(argv[6]);
    double eps_max = std::atof(argv[7]);
    double alpha = std::atof(argv[8]);
    double delta = std::atof(argv[9]);
    double gamma = std::atof(argv[10]);
    double f_min = std::atof(argv[11]);
    double f_max = std::atof(argv[12]);
    bool use_uniform = std::atoi(argv[13]) != 0;

    run(instance_amount, n, width, height, eps_step, eps_min, eps_max, alpha, delta, gamma, f_min, f_max, use_uniform);
    return 0;
}