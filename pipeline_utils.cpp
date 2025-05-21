#include "pipeline_utils.h"

void save_benchmark_results(std::vector<benchmark_result> results, std::string filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Write the header row
    file << "instance_name,instance_size,opt_sol_name,opt_cost,opt_fac_cost,opt_conn_cost,reconn_sol_name,reconn_cost,reconn_fac_cost,reconn_conn_cost,reconn_validity,no_reconn_sol_name,no_reconn_cost,no_reconn_fac_cost,no_reconn_conn_cost,no_reconn_validity" << std::endl;

    // Write each result to the CSV
    for (const auto &result : results) {
        file << std::fixed << std::setprecision(4)
             << result.instance_name << ","
             << result.instance_size << ","
             << result.opt_sol_name << ","
             << std::get<0>(result.opt_costs) + std::get<1>(result.opt_costs) << "," << std::get<0>(result.opt_costs) << "," << std::get<1>(result.opt_costs) << ","
             << result.reconn_sol_name << ","
             << std::get<0>(result.reconn_costs) + std::get<1>(result.reconn_costs) << "," << std::get<0>(result.reconn_costs) << "," << std::get<1>(result.reconn_costs) << ","
             << (result.reconn_validity ? "true" : "false") << ","
             << result.no_reconn_sol_name << ","
             << std::get<0>(result.no_reconn_costs) + std::get<1>(result.no_reconn_costs) << "," << std::get<0>(result.no_reconn_costs) << "," << std::get<1>(result.no_reconn_costs) << ","
             << (result.no_reconn_validity ? "true" : "false")
             << std::endl;
    }

    file.close();
    std::cout << "Results saved to " << filename << std::endl;
}

/*
    Generates facility location instances and runs the optimal assignment, 
    private assignment without reconnection and private assignment with reconnection.
*/
std::vector<benchmark_result> run(int instance_amount, int n, double width, double height, double eps, double alpha, double delta, double gamma, double f_min, double f_max, bool save_output){
    double lambda_daughter = gamma*pow(log(n), 2);
    double lambda_parent = n/(width*height*lambda_daughter);

    std::vector<benchmark_result> benchmark_results = std::vector<benchmark_result>(instance_amount);

    for (int i = 0; i < instance_amount; ++i){
        // generate instance
        std::vector<location> instance = generate_instance(width,height,lambda_parent,lambda_daughter,delta, f_min, f_max, 20, 4, 40);
        instance = apply_laplacian(instance, eps);

        std::string instance_name = generate_timestamped_filename("input", "input", ".in");
        save_points_to_file(instance, instance_name);
        
        std::cout << "Generated instance with " << instance.size() << " locations." << std::endl;

        benchmark_results[i] = pipeline(instance, instance_name, eps, alpha, delta, save_output);
    }

    return benchmark_results;
}

/*
    For a given instance runs all algorithms and stores their assignments.
*/
benchmark_result pipeline(std::vector<location> instance, std::string instance_name, double eps, double alpha, double delta, bool save_output){
        // run opt 
        std::vector<location> opt_out = compute_assignments(instance);
        std::tuple<double, double> opt_costs = compute_costs(opt_out);
        // std::cout << "OPT\nTotal costs: " << std::get<0>(opt_costs) + std::get<1>(opt_costs) << "\nFacility costs: " << std::get<0>(opt_costs) << "\nConnection costs: " << std::get<1>(opt_costs) << std::endl; 
        
        std::string opt_out_filename = "-";
        if(save_output){
            opt_out_filename = generate_timestamped_filename("out", "opt", ".out");
            save_results_to_file(opt_out, opt_out_filename);
        }

        // run private assignment without reconnection
        std::vector<location> no_reconn_out = private_assignment(instance, eps, alpha);
        bool no_reconn_valid = validate_solution(no_reconn_out);        
        std::tuple<double, double> no_reconn_costs = compute_costs(no_reconn_out);
        // std::cout << "Private assignment without reconnection\nValid: " << no_reconn_valid << "\nTotal costs: " << std::get<0>(no_reconn_costs) + std::get<1>(no_reconn_costs) << "\nFacility costs: " << std::get<0>(no_reconn_costs) << "\nConnection costs: " << std::get<1>(no_reconn_costs) << std::endl; 

        std::string no_reconn_out_filename = "-";
        if(save_output){
            no_reconn_out_filename = generate_timestamped_filename("out", "no_reconn", ".out");
            save_results_to_file(no_reconn_out, no_reconn_out_filename);
        }

        // run private assignment with reconnection
        std::vector<location> reconn_out = private_reconnection_assignment(instance, eps, alpha, delta);
        bool reconn_valid = validate_solution(reconn_out);
        std::tuple<double, double> reconn_costs = compute_costs(reconn_out);
        // std::cout << "Private assignment with reconnection\nValid: " << reconn_valid << "\nTotal costs: " << std::get<0>(reconn_costs) + std::get<1>(reconn_costs) << "\nFacility costs: " << std::get<0>(reconn_costs) << "\nConnection costs: " << std::get<1>(reconn_costs) << std::endl; 

        std::string reconn_out_filename = "-";
        if(save_output){
            reconn_out_filename = generate_timestamped_filename("out", "reconn", ".out");
            save_results_to_file(reconn_out, reconn_out_filename);
        }

        return benchmark_result{instance_name, instance.size(), opt_out_filename, opt_costs, reconn_out_filename, reconn_costs, reconn_valid, no_reconn_out_filename, no_reconn_costs, no_reconn_valid};
}  
