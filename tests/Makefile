CXX = g++
CXXFLAGS = -std=c++17 -Wall -I../src -I../src/maze -I../src/algorithms -I../src/mazetypes
LDFLAGS = 

# Source files
TEST_SOURCES = test_maze.cpp
MAZE_SOURCES = ../src/maze/maze.cpp \
               ../src/maze/cellborder.cpp \
               ../src/mazetypes/rectangularmaze.cpp \
               ../src/mazetypes/circularmaze.cpp \
               ../src/mazetypes/hexagonalmaze.cpp \
               ../src/algorithms/spanningtreealgorithm.cpp \
               ../src/algorithms/depthfirstsearch.cpp \
               ../src/algorithms/breadthfirstsearch.cpp \
               ../src/algorithms/kruskal.cpp

# Object files
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)
MAZE_OBJECTS = $(MAZE_SOURCES:.cpp=.o)

# Executable
TARGET = run_tests

all: $(TARGET)

$(TARGET): $(TEST_OBJECTS) $(MAZE_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TEST_OBJECTS) $(MAZE_OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run