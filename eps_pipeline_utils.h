#include "utils.h"
#include "generate.h"
#include "opt.h"
#include "privacy.h"

struct eps_benchmark_result
{
    std::string instance_name;
    double opt_costs;
    std::map<double, double> no_reconn_costs;
    std::map<double, double> reconn_costs;
};

eps_benchmark_result run_eps(std::vector<location> instance, double eps_step, double eps_min, double eps_max, double alpha, double delta);
void save_eps_benchmark_results(std::vector<eps_benchmark_result> results);
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
         bool use_uniform);