# Facility Location Problem under Local Differential Privacy without Super-set Assumption

This repository contains the source code and experimental setup for the Master's Thesis titled **"Facility Location Problem under Local Differential Privacy without Super-set Assumption"** submitted to the Technical University of Munich.

---

## ⚙️ Build Instructions

To build the project, ensure you have a C++ compiler with C++11 support or higher. Then, simply run:

```bash
make
```

## 📊 Running Experiments

Each experiment is organized in its corresponding folder. You can run benchmarks for different privacy parameters or data sizes. For example:

```bash
# Compile benchmark
make delta/delta_benchmark

# Run delta benchmark
./delta_benchmark.bin [args]
```


## 📈 Plotting Results

After running experiments, you can use the provided shell scripts to generate output plots (requires gnuplot or similar)
```bash
./plot_out.sh out.csv
```
or to plot the input with
```bash
./plot.sh in.csv
```

## 📄 Thesis Abstract
In this paper, we introduce an adaptation of the facility location problem and analyze it within the framework of local differential
privacy (LDP). Under this model, we ensure the privacy of client presence at specific locations. When n is the number of points, Gupta et
al. established a lower bound of Ω(√n) on the approximation ratio for any differentially private algorithm applied to the original facility location problem. As a result, subsequent works have adopted the super-set assumption, which may, however, compromise user privacy. We show that this lower bound does not apply to our adaptation by presenting
an LDP algorithm that achieves a constant approximation ratio with a relatively small additive factor. Additionally, we provide experimental results demonstrating that our algorithm outperforms the straightforward
approach on both synthetically generated and real-world datasets.