#include "utils.h"
#include "generate.h"
#include "opt.h"
#include "privacy.h"

struct delta_benchmark_result
{
    std::string instance_name;
    double opt_costs;
    double no_reconn_costs;
    std::map<double, double> reconn_costs;
    double best_delta;
    std::vector<location> best_reconn_out;
};

void save_delta_benchmark_results(std::vector<delta_benchmark_result> results);
delta_benchmark_result run_delta(std::vector<location> instance, double eps, double alpha, double delta_step, double max_delta);
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
         bool use_uniform);