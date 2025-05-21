#include "pipeline_utils.h"

struct gamma_benchmark_result
{
    double gamma;
    double opt_costs;
    double no_reconn_costs;
    double reconn_costs;
};

void save_benchmark_results(std::vector<gamma_benchmark_result> results, std::string filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Write the header row
    file << "gamma,opt_costs,no_reconn_costs,reconn_costs" << std::endl;

    // Write each result to the CSV
    for (const auto &result : results)
    {
        file << std::fixed << std::setprecision(4)
             << result.gamma << ","
             << result.opt_costs << ","
             << result.no_reconn_costs << ","
             << result.reconn_costs << std::endl;
    }

    file.close();
    std::cout << "Results saved to " << filename << std::endl;
}

void run_gamma(
    int instance_amount,
    int n,
    double width,
    double height,
    double eps,
    double alpha,
    double delta,
    double gamma_min,
    double gamma_max,
    double gamma_step,
    double f_min,
    double f_max)
{
    std::vector<gamma_benchmark_result> benchmark_results;

    for (double curr_gamma = gamma_min; curr_gamma <= gamma_max; curr_gamma += gamma_step)
    {
        // run pipeline
        std::vector<benchmark_result> results = run(
            instance_amount,
            n,
            width,
            height,
            eps,
            alpha,
            delta,
            curr_gamma,
            f_min,
            f_max,
            false);

        double avg_opt = 0;
        double avg_no_reconn = 0;
        double avg_reconn = 0;

        for (benchmark_result &result : results)
        {
            avg_opt += std::get<0>(result.opt_costs) + std::get<1>(result.opt_costs);
            avg_no_reconn += std::get<0>(result.no_reconn_costs) + std::get<1>(result.no_reconn_costs);
            avg_reconn += std::get<0>(result.reconn_costs) + std::get<1>(result.reconn_costs);
        }

        avg_opt /= results.size();
        avg_no_reconn /= results.size();
        avg_reconn /= results.size();

        benchmark_results.push_back(gamma_benchmark_result{curr_gamma, avg_opt, avg_no_reconn, avg_reconn});
    }

    // store results
    std::string result_filename = generate_timestamped_filename("gamma_benchmark_out", std::to_string(gamma_min) + "_" + std::to_string(gamma_max) + "_" + std::to_string(gamma_step), ".csv");
    save_benchmark_results(benchmark_results, result_filename);
}

int main(int argc, char *argv[])
{
    // Check for the correct number of arguments
    if (argc < 12)
    {
        std::cerr << "Usage: " << argv[0] << " <instance_amount> <n> <width> <height> <eps> <alpha> <delta> <gamma_min> <gamma_max> <gamma_step> <f_min> <f_max>" << std::endl;
        return 1;
    }

    // Parse arguments
    int instance_amount = std::atoi(argv[1]);
    int n = std::atoi(argv[2]);
    double width = std::atoi(argv[3]);
    double height = std::atof(argv[4]);
    double eps = std::atof(argv[5]);
    double alpha = std::atof(argv[6]);
    double delta = std::atof(argv[7]);
    double gamma_min = std::atof(argv[8]);
    double gamma_max = std::atof(argv[9]);
    double gamma_step = std::atof(argv[10]);
    double f_min = std::atof(argv[11]);
    double f_max = std::atof(argv[12]);

    run_gamma(instance_amount, n, width, height, eps, alpha, delta, gamma_min, gamma_max, gamma_step, f_min, f_max);

    return 0;
}