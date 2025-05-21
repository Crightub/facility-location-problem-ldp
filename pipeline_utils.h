#include "utils.h"
#include "generate.h"
#include "opt.h"
#include "privacy.h"

struct benchmark_result{
    std::string instance_name;
    unsigned long instance_size;
    std::string opt_sol_name;
    std::tuple<double, double> opt_costs;
    std::string reconn_sol_name;
    std::tuple<double, double> reconn_costs;
    bool reconn_validity;
    std::string no_reconn_sol_name;
    std::tuple<double, double> no_reconn_costs;
    bool no_reconn_validity;
};

void save_benchmark_results(std::vector<benchmark_result> results, std::string filename);
std::vector<benchmark_result> run(int instance_amount, int n, double width, double height, double eps, double alpha, double delta, double gamma, double f_min, double f_max, bool save_output);
benchmark_result pipeline(std::vector<location> instance, std::string instance_name, double eps, double alpha, double delta, bool save_output);