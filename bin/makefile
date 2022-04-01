CPP_FLAGS = -std=c++17 -pthread
SOURCE = ../src/main.cpp
DEPENDENCIAS = ../src/parse_utils/parse.cpp ../src/algorithms/algorithms.cpp ../src/algorithms/alg_utils_sg.cpp ../src/algorithms/brute_force.cpp
OUTPUTS = ./outputs/output*
MAIN = ./pmt

all: config

config:
	g++ $(CPP_FLAGS) $(SOURCE) $(DEPENDENCIAS) -o $(MAIN)

clean:
	rm $(OUTPUTS)
	rm $(MAIN)


