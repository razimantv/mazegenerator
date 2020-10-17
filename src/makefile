CXX ?= g++
CXXFLAGS = -W -Wall -Wno-unused-result -O3 -I algorithms -I mazetypes -I maze \
	-std=c++11

all: mazegen

OBJECTS = \
	algorithms/breadthfirstsearch.o algorithms/depthfirstsearch.o \
	algorithms/kruskal.o algorithms/looperasedrandomwalk.o \
	algorithms/spanningtreealgorithm.o algorithms/prim.o \
	mazetypes/circularhexagonmaze.o mazetypes/circularmaze.o \
	mazetypes/hexagonalmaze.o mazetypes/honeycombmaze.o \
	mazetypes/rectangularmaze.o mazetypes/usermaze.o \
	maze/cellborder.o  maze/maze.o \
	main.o

mazegen: $(OBJECTS)
	@echo "Building" $@
	@$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp
	@echo "Compiling" $<
	@$(CXX) -o $@ -c $< $(CXXFLAGS)

.phony: clean
clean:
	@echo "Cleaning"
	@rm -f $(OBJECTS) mazegen
