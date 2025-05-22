#include "eps_pipeline_utils.h"

void save_eps_benchmark_results(std::vector<eps_benchmark_result> results, std::string filename)
{
    std::ofstream csv_file(filename);
    if (!csv_file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    // Write CSV header
    csv_file << "instance_name,algorithm,eps,cost\n";

    for (const eps_benchmark_result &result : results)
    {  
        // write opt costs
        for (const auto &[eps, _] : result.no_reconn_costs)
        {
            csv_file << result.instance_name << ",opt," << eps << "," << result.opt_costs << "\n";
        }

        // write no reconnection costs
        for (const auto &[eps, cost] : result.no_reconn_costs)
        {
            csv_file << result.instance_name << ",no_reconn," << eps << "," << cost << "\n";
        }

        // write reconnection costs
        for (const auto &[eps, cost] : result.reconn_costs)
        {
            csv_file << result.instance_name << ",reconn," << eps << "," << cost << "\n";
        }
    }

    csv_file.close();
    std::cout << "Saved benchmark results to " << filename << std::endl;
}

eps_benchmark_result run_eps(std::vector<location> instance, double eps_step, double eps_min, double eps_max, double alpha, double delta)
{
    eps_benchmark_result result = eps_benchmark_result{};

    // compute optimal assignment -> independet of eps
    std::vector<location> opt_out = compute_assignments(instance);
    std::tuple<double, double> opt_costs = compute_costs(opt_out);
    result.opt_costs = std::get<0>(opt_costs) + std::get<1>(opt_costs);

    std::string filename = generate_timestamped_filename("out", "opt_out", ".out");
    save_results_to_file(opt_out, filename);

    for (double curr_eps = eps_min; curr_eps <= eps_max; curr_eps += eps_step)
    {
        // Update instance to for new privacy budget
        instance = apply_laplacian(instance, curr_eps);

        // Compute no reconnection out
        std::vector<location> no_reconn_out = private_assignment(instance, curr_eps, alpha);
        // bool no_reconn_valid = validate_solution(no_reconn_out);
        std::tuple<double, double> no_reconn_costs = compute_costs(no_reconn_out);
        result.no_reconn_costs[curr_eps] = std::get<0>(no_reconn_costs) + std::get<1>(no_reconn_costs);

        // Compute reconnection out
        std::vector<location> reconn_out = private_reconnection_assignment(instance, curr_eps, alpha, delta);
        bool reconn_valid = validate_solution(reconn_out);
        if (!reconn_valid)
        {
            // TODO: Store this in a better way
            result.reconn_costs[curr_eps] = -1;
            continue;
        }

        std::tuple<double, double> reconn_costs = compute_costs(reconn_out);
        result.reconn_costs[curr_eps] = std::get<0>(reconn_costs) + std::get<1>(reconn_costs);
    }

    return result;
}

void run(int instance_amount,
         int n,
         double width,
         double height,
         double eps_step,
         double eps_min,
         double eps_max,
         double alpha,
         double delta,
         double gamma,
         double f_min,
         double f_max,
         bool use_uniform)
{
    std::vector<eps_benchmark_result> results = std::vector<eps_benchmark_result>(instance_amount);
    for (int i = 0; i < instance_amount; ++i)
    {
        // Generate instance
        std::vector<location> instance;
        if (use_uniform)
        {
            instance = generate_instance_uniform(width, height, n, f_min, f_max, 2.5, 1.5, 8);
        }
        else
        {
            double lambda_daughter = gamma * pow(log(n), 2);
            double lambda_parent = n / lambda_daughter;
            instance = generate_instance(width, height, lambda_parent, lambda_daughter, delta, f_min, f_max, 2.5, 1.5, 8);
        }

        // store instance
        std::string instance_name = generate_timestamped_filename("input", "input", ".in");
        save_points_to_file(instance, instance_name);

        eps_benchmark_result result = run_eps(instance, eps_step, eps_min, eps_max, alpha, delta);
        result.instance_name = instance_name;
        
        results[i] = result;
    }

    // Store benchmark results
    std::string base_name = std::to_string(eps_min) + "_" + std::to_string(eps_max) + "_" + std::to_string(eps_step);
    std::string filename = generate_timestamped_filename("eps/out", base_name, ".csv");
    save_eps_benchmark_results(results, filename);
}