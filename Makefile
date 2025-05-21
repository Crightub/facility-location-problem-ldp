CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3
EXT = .bin

# Targets
all: opt generate privacy

opt$(EXT): opt.o utils.o
	$(CXX) $(CXXFLAGS) -o opt opt.o utils.o

generate$(EXT): generate.o utils.o
	$(CXX) $(CXXFLAGS) -o generate generate.o utils.o

privacy$(EXT): privacy.o utils.o
	$(CXX) $(CXXFLAGS) -o privacy privacy.o utils.o

test$(EXT): test.o utils.o privacy.o
	$(CXX) $(CXXFLAGS) -o test test.o utils.o privacy.o

pipeline_utils$(EXT): pipeline_utils.o generate.o utils.o opt.o privacy.o
	$(CXX) $(CXXFLAGS) -o pipeline_utils pipeline_utils.o generate.o utils.o opt.o privacy.o

pipeline$(EXT): pipeline.o pipeline_utils.o utils.o generate.o opt.o privacy.o
	$(CXX) $(CXXFLAGS) -o pipeline pipeline.o pipeline_utils.o utils.o generate.o opt.o privacy.o

benchmark$(EXT): benchmark.o pipeline_utils.o utils.o generate.o privacy.o opt.o
	$(CXX) $(CXXFLAGS) -o benchmark benchmark.o pipeline_utils.o utils.o generate.o privacy.o opt.o

delta_pipeline_utils$(EXT): delta_pipeline_utils.o utils.o opt.o privacy.o generate.o
	$(CXX) $(CXXFLAGS) -o delta_pipeline_utils delta_pipeline_utils.o utils.o opt.o privacy.o generate.o

delta_pipeline$(EXT): delta_pipeline.o delta_pipeline_utils.o utils.o opt.o privacy.o generate.o
	$(CXX) $(CXXFLAGS) -o delta_pipeline delta_pipeline.o delta_pipeline_utils.o utils.o opt.o privacy.o generate.o

delta_benchmark$(EXT): delta_benchmark.o delta_pipeline_utils.o utils.o opt.o privacy.o generate.o
	$(CXX) $(CXXFLAGS) -o delta_benchmark delta_benchmark.o delta_pipeline_utils.o utils.o opt.o privacy.o generate.o

eps_pipeline_utils$(EXT): eps_pipeline_utils.o utils.o opt.o privacy.o generate.o
	$(CXX) $(CXXFLAGS) -o eps_pipeline_utils eps_pipeline_utils.o utils.o opt.o privacy.o generate.o

eps_pipeline$(EXT): eps_pipeline.o eps_pipeline_utils.o utils.o opt.o privacy.o generate.o
	$(CXX) $(CXXFLAGS) -o eps_pipeline eps_pipeline.o eps_pipeline_utils.o utils.o opt.o privacy.o generate.o

eps_benchmark$(EXT): eps_benchmark.o eps_pipeline_utils.o utils.o opt.o privacy.o generate.o
	$(CXX) $(CXXFLAGS) -o eps_benchmark eps_benchmark.o eps_pipeline_utils.o utils.o opt.o privacy.o generate.o

real_world_benchmark$(EXT): real_world_benchmark.o utils.o delta_pipeline_utils.o generate.o opt.o privacy.o
	$(CXX) $(CXXFLAGS) -o real_world_benchmark real_world_benchmark.o delta_pipeline_utils.o utils.o generate.o opt.o privacy.o

prep_data$(EXT): prep_data.o utils.o
	$(CXX) $(CXXFLAGS) -o prep_data prep_data.o utils.o

b_benchmark$(EXT): b_benchmark.o pipeline_utils.o utils.o generate.o opt.o privacy.o
	$(CXX) $(CXXFLAGS) -o b_benchmark b_benchmark.o pipeline_utils.o utils.o generate.o opt.o privacy.o

gamma_benchmark$(EXT): gamma_benchmark.o pipeline_utils.o utils.o generate.o opt.o privacy.o
	$(CXX) $(CXXFLAGS) -o gamma_benchmark gamma_benchmark.o pipeline_utils.o utils.o generate.o opt.o privacy.o

opt.o: opt.cpp utils.h
	$(CXX) $(CXXFLAGS) -c opt.cpp

generate.o: generate.cpp utils.h
	$(CXX) $(CXXFLAGS) -c generate.cpp

privacy.o: privacy.cpp utils.h
	$(CXX) $(CXXFLAGS) -c privacy.cpp

utils.o: utils.cpp utils.h
	$(CXX) $(CXXFLAGS) -c utils.cpp

pipeline_utils.o: pipeline_utils.cpp generate.h utils.h opt.h privacy.h
	$(CXX) $(CXXFLAGS) -c pipeline_utils.cpp

pipeline.o: pipeline.cpp pipeline_utils.cpp
	$(CXX) $(CXXFLAGS) -c pipeline.cpp

benchmark.o: benchmark.cpp pipeline_utils.cpp
	$(CXX) $(CXXFLAGS) -c benchmark.cpp

delta_pipeline.o: delta_pipeline.cpp delta_pipeline_utils.cpp
	$(CXX) $(CXXFLAGS) -c delta_pipeline.cpp

delta_benchmark.o: delta_benchmark.cpp delta_pipeline.cpp
	$(CXX) $(CXXFLAGS) -c delta_benchmark.cpp

delta_pipeline_utils.o: delta_pipeline_utils.cpp utils.h opt.h privacy.h generate.h
	$(CXX) $(CXXFLAGS) -c delta_pipeline_utils.cpp

eps_pipeline.o: eps_pipeline.cpp eps_pipeline_utils.cpp
	$(CXX) $(CXXFLAGS) -c eps_pipeline.cpp

eps_benchmark.o: eps_benchmark.cpp eps_pipeline.cpp
	$(CXX) $(CXXFLAGS) -c eps_benchmark.cpp

eps_pipeline_utils.o: eps_pipeline_utils.cpp utils.h opt.h privacy.h generate.h
	$(CXX) $(CXXFLAGS) -c eps_pipeline_utils.cpp

test.o: test.cpp privacy.h utils.h
	$(CXX) $(CXXFLAGS) -c test.cpp

prep_data.o: prep_data.cpp utils.h
	$(CXX) $(CXXFLAGS) -c prep_data.cpp

real_world_benchmark.o: real_world_benchmark.cpp utils.h delta_pipeline_utils.h generate.h opt.h privacy.h
	$(CXX) $(CXXFLAGS) -c real_world_benchmark.cpp

b_benchmark.o: b_benchmark.cpp pipeline_utils.cpp
	$(CXX) $(CXXFLAGS) -c b_benchmark.cpp

gamma_benchmark.o: gamma_benchmark.cpp pipeline_utils.cpp
	$(CXX) $(CXXFLAGS) -c gamma_benchmark.cpp

# Clean up build files
clean:
	rm -f *.o *.$(EXT)
