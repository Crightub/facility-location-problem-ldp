CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3
EXT = .bin

# Targets
all: opt$(EXT) generate$(EXT) privacy$(EXT)

# Clean up build and executable files
clean:
	rm -f *.o *.$(EXT)

# ------ General Files ------

# Executable Files
opt$(EXT): opt.o utils.o
	$(CXX) $(CXXFLAGS) -o opt$(EXT) opt.o utils.o

generate$(EXT): generate.o utils.o
	$(CXX) $(CXXFLAGS) -o generate$(EXT) generate.o utils.o

privacy$(EXT): privacy.o utils.o
	$(CXX) $(CXXFLAGS) -o privacy$(EXT) privacy.o utils.o

test$(EXT): test.o utils.o privacy.o
	$(CXX) $(CXXFLAGS) -o test$(EXT) test.o utils.o privacy.o

# Object Files
opt.o: opt.cpp utils.h
	$(CXX) $(CXXFLAGS) -c opt.cpp

generate.o: generate.cpp utils.h
	$(CXX) $(CXXFLAGS) -c generate.cpp

privacy.o: privacy.cpp utils.h
	$(CXX) $(CXXFLAGS) -c privacy.cpp

utils.o: utils.cpp utils.h
	$(CXX) $(CXXFLAGS) -c utils.cpp

test.o: test.cpp privacy.h utils.h
	$(CXX) $(CXXFLAGS) -c test.cpp

# ------ Algorithm Pipeline ------

# Executable Files
pipeline_utils$(EXT): pipeline_utils.o generate.o utils.o opt.o privacy.o
	$(CXX) $(CXXFLAGS) -o pipeline_utils$(EXT) pipeline_utils.o generate.o utils.o opt.o privacy.o

pipeline$(EXT): pipeline.o pipeline_utils.o utils.o generate.o opt.o privacy.o
	$(CXX) $(CXXFLAGS) -o pipeline$(EXT) pipeline.o pipeline_utils.o utils.o generate.o opt.o privacy.o

# Object Files
pipeline_utils.o: pipeline_utils.cpp generate.h utils.h opt.h privacy.h
	$(CXX) $(CXXFLAGS) -c pipeline_utils.cpp

pipeline.o: pipeline.cpp pipeline_utils.cpp
	$(CXX) $(CXXFLAGS) -c pipeline.cpp

# ------ Instance Size (n) Benchmark ------

# Executable Files
benchmark$(EXT): instance_size_n/benchmark.o pipeline_utils.o utils.o generate.o privacy.o opt.o
	$(CXX) $(CXXFLAGS) -o benchmark$(EXT) instance_size_n/benchmark.o pipeline_utils.o utils.o generate.o privacy.o opt.o

# Instance Size (n) Benchmark Object Files
instance_size_n/benchmark.o: instance_size_n/benchmark.cpp pipeline_utils.cpp
	$(CXX) $(CXXFLAGS) -c instance_size_n/benchmark.cpp

# ------ Delta Benchmark & Pipeline ------

# Executable Files
delta_pipeline_utils$(EXT): delta_pipeline_utils.o utils.o opt.o privacy.o generate.o
	$(CXX) $(CXXFLAGS) -o delta_pipeline_utils$(EXT) delta_pipeline_utils.o utils.o opt.o privacy.o generate.o

delta_pipeline$(EXT): delta/delta_pipeline.o delta/delta_pipeline_utils.o utils.o opt.o privacy.o generate.o
	$(CXX) $(CXXFLAGS) -o delta_pipeline$(EXT) delta/delta_pipeline.o delta/delta_pipeline_utils.o utils.o opt.o privacy.o generate.o

delta_benchmark$(EXT): delta/delta_benchmark.o delta/delta_pipeline_utils.o utils.o opt.o privacy.o generate.o
	$(CXX) $(CXXFLAGS) -o delta_benchmark$(EXT) delta/delta_benchmark.o delta/delta_pipeline_utils.o utils.o opt.o privacy.o generate.o

# Object Files
delta/delta_pipeline.o: delta/delta_pipeline.cpp delta/delta_pipeline_utils.h
	$(CXX) $(CXXFLAGS) -c delta/delta_pipeline.cpp -o delta/delta_pipeline.o

delta/delta_pipeline_utils.o: delta/delta_pipeline_utils.cpp delta/delta_pipeline_utils.h
	$(CXX) $(CXXFLAGS) -c delta/delta_pipeline_utils.cpp -o delta/delta_pipeline_utils.o

delta/delta_benchmark.o: delta/delta_benchmark.cpp
	$(CXX) $(CXXFLAGS) -c delta/delta_benchmark.cpp -o delta/delta_benchmark.o

# ------ Epsilon Benchmark & Pipeline ------

# Executable Files
eps_pipeline_utils$(EXT): eps/eps_pipeline_utils.o utils.o opt.o privacy.o generate.o
	$(CXX) $(CXXFLAGS) -o eps_pipeline_utils$(EXT) eps/eps_pipeline_utils.o utils.o opt.o privacy.o generate.o

eps_pipeline$(EXT): eps/eps_pipeline.o eps/eps_pipeline_utils.o utils.o opt.o privacy.o generate.o
	$(CXX) $(CXXFLAGS) -o eps_pipeline$(EXT) eps/eps_pipeline.o eps/eps_pipeline_utils.o utils.o opt.o privacy.o generate.o

eps_benchmark$(EXT): eps_benchmark.o eps_pipeline_utils.o utils.o opt.o privacy.o generate.o
	$(CXX) $(CXXFLAGS) -o eps_benchmark$(EXT) eps_benchmark.o eps_pipeline_utils.o utils.o opt.o privacy.o generate.o

# Object Files
eps/eps_pipeline.o: eps/eps_pipeline.cpp eps/eps_pipeline_utils.cpp
	$(CXX) $(CXXFLAGS) -c eps/eps_pipeline.cpp

eps/eps_benchmark.o: eps/eps_benchmark.cpp eps/eps_pipeline.cpp
	$(CXX) $(CXXFLAGS) -c eps/eps_benchmark.cpp

eps/eps_pipeline_utils.o: eps/eps_pipeline_utils.cpp utils.h opt.h privacy.h generate.h
	$(CXX) $(CXXFLAGS) -c eps/eps_pipeline_utils.cpp

# ------ Real World Benchmark & Pipeline ------

# Executable Files
real_world_benchmark$(EXT): real_world/real_world_benchmark.o delta/delta_pipeline_utils.o utils.o  generate.o opt.o privacy.o
	$(CXX) $(CXXFLAGS) -o real_world_benchmark$(EXT) real_world/real_world_benchmark.o delta/delta_pipeline_utils.o utils.o generate.o opt.o privacy.o

prep_data$(EXT): real_world/prep_data.o utils.o
	$(CXX) $(CXXFLAGS) -o prep_data$(EXT) real_world/prep_data.o utils.o

# Object Files
real_world/prep_data.o: real_world/prep_data.cpp utils.h
	$(CXX) $(CXXFLAGS) -c prep_data.cpp

real_world/real_world_benchmark.o: real_world/real_world_benchmark.cpp utils.h delta/delta_pipeline_utils.h generate.h opt.h privacy.h
	$(CXX) $(CXXFLAGS) -c real_world_benchmark.cpp

# ------ Average Clients(B) Benchmark & Pipeline ------

# Executable Files
b_benchmark$(EXT): clients_b/b_benchmark.o pipeline_utils.o utils.o generate.o opt.o privacy.o
	$(CXX) $(CXXFLAGS) -o clients_b/b_benchmark$(EXT) clients_b/b_benchmark.o pipeline_utils.o utils.o generate.o opt.o privacy.o

# Object Files
clients_b/b_benchmark.o: clients_b/b_benchmark.cpp pipeline_utils.cpp
	$(CXX) $(CXXFLAGS) -c clients_b/b_benchmark.cpp

# ------ Gamma Benchmark & Pipeline ------

# Executable Files
gamma_benchmark$(EXT): gamma/gamma_benchmark.o pipeline_utils.o utils.o generate.o opt.o privacy.o
	$(CXX) $(CXXFLAGS) -o gamma_benchmark$(EXT) gamma/gamma_benchmark.o pipeline_utils.o utils.o generate.o opt.o privacy.o

# Object Files
gamma/gamma_benchmark.o: gamma/gamma_benchmark.cpp pipeline_utils.cpp
	$(CXX) $(CXXFLAGS) -c gamma/gamma_benchmark.cpp

