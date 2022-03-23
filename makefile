CPP_FLAGS = -std=c++17
SOURCE = ./src/main.cpp
DEPENDENCIAS = ./src/parse.cpp
MAIN = ./bin/main

all: config

config:
	g++ $(CPP_FLAGS) $(SOURCE) $(DEPENDENCIAS) -o $(MAIN)

clean:
	rm $(MAIN)

