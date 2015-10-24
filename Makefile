CC = g++
CXXFLAGS = -O3 -W -Wall -Wno-unused-result -std=gnu++11

OBJS = dfs.o kruskal.o maze.o mazebase.o prim.o subdivide.o

all: maze

maze: $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) -o maze

%.o: %.cpp
	$(CC) $(CXXFLAGS) $< -c -o $@

.PHONY: clean
clean:
	rm -f *.o maze
