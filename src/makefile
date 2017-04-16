CXX ?= g++
CXXFLAGS = -W -Wall -Wno-unused-result -O3 -std=c++11

all: mazegen

OBJECTS = breadthfirstsearch.o cellborder.o circularhexagonmaze.o \
	circularmaze.o depthfirstsearch.o hexagonalmaze.o honeycombmaze.o \
	kruskal.o main.o maze.o minimumspanningtreealgorithm.o rectangularmaze.o

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
