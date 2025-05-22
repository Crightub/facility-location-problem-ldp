# Facility Location Problem under Local Differential Privacy without Super-set Assumption

This repository contains the source code and experimental setup for the Master's Thesis titled **"Facility Location Problem under Local Differential Privacy without Super-set Assumption"** submitted to the Technical University of Munich.

---

## ⚙️ Build Instructions

To build the project, ensure you have a C++ compiler with C++17 support or higher. Then, simply run the make comment for every experiment:

Each experiment is organized in its corresponding folder:
### instance size benchmark 
📁 instance_size_n/
```bash
make benchmark
```

### delta benchmark
📁 delta/
```bash
make delta_benchmark
```

### epsilon benchmark
📁 eps/
```bash
make eps_benchmark
```

### gamma benchmark
📁 gamma/
```bash
make gamma_benchmark
```

### clients benchmark
📁 clients_b/
```bash
make b_benchmark
```

### real world benchmark
📁 real_world/
```bash
make real_world_benchmark
```

## 📊 Running Experiments

Before running any experiments use the make command to compile it. Afterwards, run the executable with the desired arguments. Executing an experiments without any or wrongly entered arguements will print out information about the required arguements. 

For example:

```bash
# Compile benchmark
make delta_benchmark

# Run delta benchmark
./delta_benchmark.bin [args]

# delta benchmark help output
Usage: ./delta_benchmark.bin <instance_amount> <n> <width> <height> <delta> <delta_step> <eps> <alpha> <gamma> <f_min> <f_max> <use_uniform>
```

Benchmarks will write the experiments to a csv file.

## 📈 Plotting Instances

After running experiments, you can use the provided shell scripts to generate output plots (requires gnuplot or similar)
```bash
./plot_out.sh out.csv
```
or to plot the input instance with
```bash
./plot.sh in.csv
```

Please use other available programs such as Pyhton Matplotlib, Microsoft Excel, etc. to visualize the experiment results.

## 📄 Thesis Abstract
In this paper, we introduce an adaptation of the facility location problem and analyze it within the framework of local differential
privacy (LDP). Under this model, we ensure the privacy of client presence at specific locations. When n is the number of points, Gupta et
al. established a lower bound of Ω(√n) on the approximation ratio for any differentially private algorithm applied to the original facility location problem. As a result, subsequent works have adopted the super-set assumption, which may, however, compromise user privacy. We show that this lower bound does not apply to our adaptation by presenting
an LDP algorithm that achieves a constant approximation ratio with a relatively small additive factor. Additionally, we provide experimental results demonstrating that our algorithm outperforms the straightforward
approach on both synthetically generated and real-world datasets.