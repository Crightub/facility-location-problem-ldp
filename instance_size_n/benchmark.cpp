#include "pipeline_utils.h"

int main(int argc, char *argv[])
{
    // Check for the correct number of arguments
    if (argc < 13 || argc > 14)
    {
        std::cerr << "Usage: " << argv[0] << " <instance_amount> <n_step> <n_min> <n_max> <width> <height> <eps> <alpha> <delta> <gamma> <f_min> <f_max> [-s]" << std::endl;
        return 1;
    }

    // Parse arguments
    int instance_amount = std::atoi(argv[1]);
    int n_step = std::atoi(argv[2]);
    int n_min = std::atoi(argv[3]);
    int n_max = std::atoi(argv[4]);
    double width = std::atof(argv[5]);
    double height = std::atof(argv[6]);
    double eps = std::atof(argv[7]);
    double alpha = std::atof(argv[8]);
    double delta = std::atof(argv[9]);
    double gamma = std::atof(argv[10]);
    double f_min = std::atof(argv[11]);
    double f_max = std::atof(argv[12]);

    // Check for optional -s flag
    bool save_output = (argc == 14 && std::string(argv[13]) == "-s");

    // Validate arguments
    if (instance_amount <= 0 || delta <= 0 || eps <= 0 || alpha <= 0 || f_min < 0 || f_max < 0)
    {
        std::cerr << "All arguments must be positive numbers." << std::endl;
        return 1;
    }

    std::vector<benchmark_result> benchmark_results;

    for (int curr_n = n_min; curr_n <= n_max; curr_n += n_step)
    {
        std::cout << "Run pipeline for n=" << curr_n << "..." << std::endl;
        std::vector<benchmark_result> results = run(instance_amount, curr_n, width, height, eps, alpha, delta, gamma, f_min, f_max, save_output);

        double avg_opt_fac = 0;
        double avg_opt_conn = 0;
        double avg_no_reconn_fac = 0;
        double avg_no_reconn_conn = 0;
        double avg_reconn_fac = 0;
        double avg_reconn_conn = 0;

        for (benchmark_result &result : results)
        {
            avg_opt_fac += std::get<0>(result.opt_costs);
            avg_opt_conn += std::get<1>(result.opt_costs);
            avg_no_reconn_fac += std::get<0>(result.no_reconn_costs);
            avg_no_reconn_conn += std::get<1>(result.no_reconn_costs);
            avg_reconn_fac += std::get<0>(result.reconn_costs);
            avg_reconn_conn += std::get<1>(result.reconn_costs);
        }

        avg_opt_fac /= results.size();
        avg_opt_conn /= results.size();
        avg_no_reconn_fac /= results.size();
        avg_no_reconn_conn /= results.size();
        avg_reconn_fac /= results.size();
        avg_reconn_conn /= results.size();

        benchmark_results.push_back(benchmark_result{"",
                                                     static_cast<unsigned long>(curr_n),
                                                     "-",
                                                     std::tuple<double, double>{avg_opt_fac, avg_opt_conn},
                                                     "-",
                                                     std::tuple<double, double>{avg_reconn_fac, avg_reconn_conn},
                                                     true,
                                                     "-",
                                                     std::tuple<double, double>{avg_no_reconn_fac, avg_no_reconn_conn},
                                                     false});

        // benchmark_results.insert(benchmark_results.end(), results.begin(), results.end());
    }

    // Store benchmark results
    std::string result_filename = generate_timestamped_filename("benchmark_out", "benchmark", ".csv");
    save_benchmark_results(benchmark_results, result_filename);

    return 0;
}