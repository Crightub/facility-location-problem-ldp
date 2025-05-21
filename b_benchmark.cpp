#include "pipeline_utils.h"

struct b_benchmark_result{
    std::string instance_name;
    double b_avg;
    double opt_costs;
    double reconn_costs;
    double no_reconn_costs;
    double normalized_costs;
};

void save_b_benchmark_results(std::vector<b_benchmark_result> results, std::string filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Write the header row
    file << "instance_name,b_avg,opt_cost,reconn_cost,no_reconn_cost,normalized_cost" << std::endl;

    // Write each result to the CSV
    for (const auto &result : results) {
        file << std::fixed << std::setprecision(4)
             << result.instance_name << ","
             << result.b_avg << ","
             << result.opt_costs << ","
             << result.reconn_costs << ","
             << result.no_reconn_costs << ","
             << result.normalized_costs << std::endl;
    }

    file.close();
    std::cout << "Results saved to " << filename << std::endl;
}


void run_b(int instance_amount, 
    int n, 
    double width, 
    double height,
    double delta, 
    double eps, 
    double alpha, 
    double gamma, 
    double f_min, 
    double f_max, 
    double b_avg_max, 
    double b_avg_step){

    double lambda_daughter = pow(gamma, 2) * pow(log(n), 2);
    double lambda_parent = n / lambda_daughter;

    std::vector<b_benchmark_result> results;

    for (int i = 0; i < instance_amount; ++i){
        std::cout << "Generate instance with number: " << i << std::endl;
        // Generate instance
        std::vector<location> instance = generate_instance(width, height, lambda_parent, lambda_daughter, delta, f_min, f_max, 1, 0, 1);
        std::string instance_name = generate_timestamped_filename("input", "input", ".in");

        for (double curr_avg = 1.0; curr_avg <= b_avg_max; curr_avg += b_avg_step){
            std::cout << "Run instance " << i << " with avg " << curr_avg << std::endl;
            // Set amount of clients
            for (location &v : instance){
                v.b = curr_avg;
            }
            // Apply laplacian
            auto noisy_instance = apply_laplacian(instance, eps);
            // Run algorithms
            benchmark_result result = pipeline(noisy_instance, instance_name, eps, alpha, delta, false);

            double opt_costs = std::get<0>(result.opt_costs) + std::get<1>(result.opt_costs);
            double reconn_costs = std::get<0>(result.reconn_costs) + std::get<1>(result.reconn_costs);
            double no_reconn_costs = std::get<0>(result.no_reconn_costs) + std::get<1>(result.no_reconn_costs);
            double normalized_costs = (reconn_costs - opt_costs) / (no_reconn_costs - opt_costs);

            results.push_back(b_benchmark_result{
                instance_name,
                curr_avg,
                opt_costs,
                reconn_costs,
                no_reconn_costs,
                normalized_costs
            });
        }
    }   

    std::string result_filename = generate_timestamped_filename("b_benchmark_out", "benchmark", ".csv");
    save_b_benchmark_results(results, result_filename);
}

int main(int argc, char *argv[])
{
    // Check for the correct number of arguments
    if (argc < 13)
    {
        std::cerr << "Usage: " << argv[0] << " <instance_amount> <n> <width> <height> <delta> <eps> <alpha> <gamma> <f_min> <f_max> <b_avg_max> <b_avg_step>" << std::endl;
        return 1;
    }

    // Parse arguments
    int instance_amount = std::atoi(argv[1]);
    int n = std::atoi(argv[2]);
    double width = std::atoi(argv[3]);
    double height = std::atof(argv[4]);
    double delta = std::atof(argv[5]);
    double eps = std::atof(argv[6]);
    double alpha = std::atof(argv[7]);
    double gamma = std::atof(argv[8]);
    double f_min = std::atof(argv[9]);
    double f_max = std::atof(argv[10]);
    double b_avg_max = std::atof(argv[11]);
    double b_avg_step = std::atof(argv[12]);

    run_b(instance_amount, n, width, height, delta, eps, alpha, gamma, f_min, f_max, b_avg_max, b_avg_step);
    return 0;
}