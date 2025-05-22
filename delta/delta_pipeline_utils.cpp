#include "./delta_pipeline_utils.h"

void save_delta_benchmark_results(std::vector<delta_benchmark_result> results, std::string filename)
{
    // Open the file for writing
    std::ofstream csv_file(filename);
    if (!csv_file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    // Write CSV header
    csv_file << "instance_name,delta,normalized_cost\n";

    // Write each instance's delta and normalized cost
    for (const delta_benchmark_result &result : results)
    {
        if (result.no_reconn_costs == 0)
        {
            continue; 
        }

        for (const auto &[delta, cost] : result.reconn_costs)
        {
            // Normalize the cost
            double normalized_cost = (cost - result.opt_costs) / (result.no_reconn_costs - result.opt_costs);
            csv_file << result.instance_name << "," << delta << "," << normalized_cost << "\n";
        }
    }

    csv_file.close();
    std::cout << "Saved benchmark results to " << filename << std::endl;
}

// Run the private reconnection algorithm for the same instance with different values for delta
delta_benchmark_result run_delta(std::vector<location> instance, double eps, double alpha, double delta_step, double max_delta)
{
    delta_benchmark_result result = {};
    // run opt
    std::vector<location> opt_out = compute_assignments(instance);
    std::tuple<double, double> opt_costs = compute_costs(opt_out);
    result.opt_costs = std::get<0>(opt_costs) + std::get<1>(opt_costs);

    std::string filename = generate_timestamped_filename("out", "opt_out", ".out");
    save_results_to_file(opt_out, filename);

    // run private no reconnection
    std::vector<location> no_reconn_out = private_assignment(instance, eps, alpha);
    // bool no_reconn_valid = validate_solution(no_reconn_out);
    std::tuple<double, double> no_reconn_costs = compute_costs(no_reconn_out);
    result.no_reconn_costs = std::get<0>(no_reconn_costs) + std::get<1>(no_reconn_costs);

    double best_delta;
    std::vector<location> best_assignment;
    double min_cost = std::numeric_limits<double>::max();

    // iterate over possible delta from 0 to max distance (simulation window size)
    for (double curr_delta = 0.0; curr_delta <= max_delta; curr_delta += delta_step)
    {
        // run private no reconnection
        std::vector<location> reconn_out = private_reconnection_assignment(instance, eps, alpha, curr_delta);
        bool reconn_valid = validate_solution(reconn_out);
        if (!reconn_valid)
        {
            // TODO: Store this in a better way
            result.reconn_costs[curr_delta] = -1;
            continue;
        }

        std::tuple<double, double> reconn_costs = compute_costs(reconn_out);
        double total_costs = std::get<0>(reconn_costs) + std::get<1>(reconn_costs);
        // std::cout << delta << "," << std::get<0>(reconn_costs) + std::get<1>(reconn_costs) << std::endl;
        result.reconn_costs[curr_delta] = total_costs;

        if (total_costs < min_cost){
            best_delta = curr_delta;
            min_cost = total_costs;
            best_assignment = reconn_out;
        }
    }

    result.best_delta = best_delta;
    result.best_reconn_out = best_assignment;

    return result;
}

// Generate instance to then run_delta over it.
void run(int instance_amount,
         int n,
         double width,
         double height,
         double delta,
         double delta_step,
         double eps,
         double alpha,
         double gamma,
         double f_min,
         double f_max,
         bool use_uniform)
{
    // use a fixed set of parameters to run the generation on
    std::vector<delta_benchmark_result> results;

    for (int i = 0; i < instance_amount; ++i)
    {
        std::cout << "Generate instance." << std::endl;

        std::vector<location> instance;
        if (use_uniform)
        {
            instance = generate_instance_uniform(width, height, n, f_min, f_max, 25, 50, 1000);
        }
        else
        {
            double lambda_daughter = pow(gamma, 2) * pow(log(n), 2);
            double lambda_parent = n / lambda_daughter;
            instance = generate_instance(width, height, lambda_parent, lambda_daughter, delta, f_min, f_max, 25, 5, 50);
        }
        auto noisy_instance = apply_laplacian(instance, eps);

        // store instance
        std::string instance_name = generate_timestamped_filename("input", "input", ".in");
        save_points_to_file(noisy_instance, instance_name);

        delta_benchmark_result result = run_delta(noisy_instance, eps, alpha, delta_step, std::max(width, height));
        result.instance_name = instance_name;

         // save best assignment
        std::string filename = generate_timestamped_filename("out", result.instance_name.substr(6, result.instance_name.size() - 3) + "_out", ".out");
        save_results_to_file(result.best_reconn_out, filename);

        results.push_back(result);
    }

    // Store benchmark results
    std::string base_name = std::to_string(delta) + "_" + std::to_string(delta_step);
    std::string filename = generate_timestamped_filename("delta/out", "delta_benchmark", ".csv");
    save_delta_benchmark_results(results, filename);
}
