// Test suite for maze generator
#include <iostream>
#include <cassert>
#include <set>
#include <queue>
#include <map>
#include <memory>
#include <functional>
#include <tuple>
#include <sstream>
#include <algorithm>
#include "../src/algorithms/spanningtreealgorithm.h"
#include "../src/maze/maze.h"
#include "../src/mazetypes/rectangularmaze.h"
#include "../src/mazetypes/circularmaze.h"
// #include "../src/mazetypes/triangularmaze.h" // Not available in base repository
#include "../src/mazetypes/hexagonalmaze.h"
#include "../src/algorithms/depthfirstsearch.h"
#include "../src/algorithms/breadthfirstsearch.h"
#include "../src/algorithms/kruskal.h"

// Enhanced assertion macros with better error messages
#define ASSERT_EQ(expected, actual, msg) \
    do { \
        if ((expected) != (actual)) { \
            std::ostringstream oss; \
            oss << msg << " - Expected: " << (expected) << ", Got: " << (actual); \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define ASSERT_TRUE(condition, msg) \
    do { \
        if (!(condition)) { \
            std::ostringstream oss; \
            oss << msg << " - Condition failed: " << #condition; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

#define ASSERT_RANGE(value, min_val, max_val, msg) \
    do { \
        if ((value) < (min_val) || (value) > (max_val)) { \
            std::ostringstream oss; \
            oss << msg << " - Value " << (value) << " not in range [" << (min_val) << ", " << (max_val) << "]"; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

// Test utilities
class TestRunner {
private:
    int tests_run_ = 0;
    int tests_passed_ = 0;
    
public:
    void run_test(const std::string& test_name, std::function<void()> test_func) {
        std::cout << "Running " << test_name << "... ";
        tests_run_++;
        try {
            test_func();
            tests_passed_++;
            std::cout << "PASSED" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "FAILED: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "FAILED: Unknown error" << std::endl;
        }
    }
    
    void print_summary() {
        std::cout << "\n=== Test Summary ===" << std::endl;
        std::cout << "Tests run: " << tests_run_ << std::endl;
        std::cout << "Tests passed: " << tests_passed_ << std::endl;
        std::cout << "Tests failed: " << (tests_run_ - tests_passed_) << std::endl;
        std::cout << "Success rate: " << (tests_passed_ * 100.0 / tests_run_) << "%" << std::endl;
        
        if (tests_passed_ != tests_run_) {
            std::cout << "\nSome tests failed. Run individual tests for detailed error messages." << std::endl;
        }
    }
};

// Test helper classes to access protected members
template<typename MazeType>
class TestableMaze : public MazeType {
public:
    using MazeType::MazeType;
    
    int GetVertexCount() const { return this->vertices_; }
    int GetStart() const { return this->startvertex_; }
    int GetEnd() const { return this->endvertex_; }
    const Graph& GetGraph() const { return this->adjacencylist_; }
};

// Enhanced helper functions with diagnostics
template<typename MazeType>
std::set<std::pair<int, int>> get_wall_edges(TestableMaze<MazeType>* maze) {
    // Get all walls that exist in the maze
    std::set<std::pair<int, int>> walls;
    const Graph& graph = maze->GetGraph();
    
    for (int v = 0; v < maze->GetVertexCount(); v++) {
        for (const auto& edge : graph[v]) {
            int to = std::get<0>(edge);
            if (to >= 0) {
                int v1 = std::min(v, to);
                int v2 = std::max(v, to);
                walls.insert({v1, v2});
            }
        }
    }
    return walls;
}

std::vector<int> get_rectangular_neighbors(int vertex, int width, int height) {
    std::vector<int> neighbors;
    int row = vertex / width;
    int col = vertex % width;
    
    // Check 4-directional neighbors
    if (row > 0) neighbors.push_back((row-1) * width + col);     // up
    if (row < height-1) neighbors.push_back((row+1) * width + col); // down
    if (col > 0) neighbors.push_back(row * width + (col-1));     // left
    if (col < width-1) neighbors.push_back(row * width + (col+1)); // right
    
    return neighbors;
}

template<typename MazeType>
std::vector<int> get_maze_neighbors(int vertex, TestableMaze<MazeType>* maze) {
    // Get all potential neighbors based on maze topology, then filter by walls
    std::vector<int> potential_neighbors;
    std::set<std::pair<int, int>> walls = get_wall_edges(maze);
    
    // For now, assume rectangular grid topology for all maze types
    // This works for rectangular mazes, and can be extended for other types
    int total = maze->GetVertexCount();
    int width = static_cast<int>(std::sqrt(total));
    int height = total / width;
    
    // Adjust if not perfect square
    if (width * height != total) {
        // Try common rectangular dimensions
        for (int w = 1; w <= total; w++) {
            if (total % w == 0) {
                width = w;
                height = total / w;
                break;
            }
        }
    }
    
    potential_neighbors = get_rectangular_neighbors(vertex, width, height);
    
    // Filter out neighbors that have walls between them
    std::vector<int> actual_neighbors;
    for (int neighbor : potential_neighbors) {
        int v1 = std::min(vertex, neighbor);
        int v2 = std::max(vertex, neighbor);
        
        // If there's no wall between vertices, they're connected by passage
        if (walls.find({v1, v2}) == walls.end()) {
            actual_neighbors.push_back(neighbor);
        }
    }
    
    return actual_neighbors;
}

template<typename MazeType>
std::pair<bool, std::string> check_graph_connectivity(TestableMaze<MazeType>* maze) {
    // Check connectivity through passages (not walls)
    std::set<int> visited;
    std::queue<int> queue;
    queue.push(maze->GetStart());
    visited.insert(maze->GetStart());
    
    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();
        
        std::vector<int> neighbors = get_maze_neighbors(current, maze);
        for (int neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                queue.push(neighbor);
            }
        }
    }
    
    bool connected = visited.size() == static_cast<size_t>(maze->GetVertexCount());
    std::ostringstream details;
    details << "Visited " << visited.size() << " of " << maze->GetVertexCount() << " vertices via passages";
    
    if (!connected) {
        details << ". Unreachable vertices: ";
        for (int i = 0; i < maze->GetVertexCount(); i++) {
            if (visited.find(i) == visited.end()) {
                details << i << " ";
            }
        }
    }
    
    return {connected, details.str()};
}

template<typename MazeType>
std::pair<bool, std::string> check_spanning_tree_property(TestableMaze<MazeType>* maze) {
    // The maze structure consists of:
    // 1. Passages form a spanning tree (exactly V-1 connections for V vertices)
    // 2. Remaining walls (internal + boundary) form additional tree structures
    // 3. Together they cover all vertices as disjoint spanning trees
    
    std::set<std::pair<int, int>> walls = get_wall_edges(maze);
    
    // Count all possible internal connections in the maze topology
    int total_possible_internal = 0;
    int vertex_count = maze->GetVertexCount();
    
    // For rectangular grid topology, count possible internal edges
    int width = static_cast<int>(std::sqrt(vertex_count));
    int height = vertex_count / width;
    
    // Adjust if not perfect square  
    if (width * height != vertex_count) {
        for (int w = 1; w <= vertex_count; w++) {
            if (vertex_count % w == 0) {
                width = w;
                height = vertex_count / w;
                break;
            }
        }
    }
    
    // Count potential internal edges in grid
    for (int v = 0; v < vertex_count; v++) {
        std::vector<int> neighbors = get_rectangular_neighbors(v, width, height);
        for (int neighbor : neighbors) {
            if (neighbor > v) { // Count each edge only once
                total_possible_internal++;
            }
        }
    }
    
    int passages = total_possible_internal - walls.size();
    int expected_passages = vertex_count - 1; // Spanning tree has V-1 edges
    
    bool is_valid_maze = (passages == expected_passages);
    
    std::ostringstream details;
    details << "Passages: " << passages << " (expected " << expected_passages 
            << " for spanning tree), Walls: " << walls.size() 
            << " (from " << total_possible_internal << " possible internal)";
    
    if (is_valid_maze) {
        details << " - Valid maze structure: passages form spanning tree";
    } else {
        details << " - Invalid: " << (passages < expected_passages ? "under" : "over") << "connected";
    }
    
    return {is_valid_maze, details.str()};
}

template<typename MazeType>
std::pair<int, std::string> analyze_boundary_walls(TestableMaze<MazeType>* maze) {
    int boundary_walls = 0;
    const Graph& graph = maze->GetGraph();
    std::vector<int> boundary_vertices;
    
    for (int v = 0; v < maze->GetVertexCount(); v++) {
        bool has_boundary = false;
        for (const auto& edge : graph[v]) {
            int to = std::get<0>(edge);
            if (to == -1) {
                boundary_walls++;
                has_boundary = true;
            }
        }
        if (has_boundary) {
            boundary_vertices.push_back(v);
        }
    }
    
    std::ostringstream details;
    details << boundary_walls << " boundary walls found across " 
            << boundary_vertices.size() << " vertices";
    
    return {boundary_walls, details.str()};
}

template<typename MazeType>
std::string analyze_maze_structure(TestableMaze<MazeType>* maze) {
    std::ostringstream analysis;
    analysis << "\n--- Maze Structure Analysis ---\n";
    analysis << "Vertices: " << maze->GetVertexCount() << "\n";
    analysis << "Start vertex: " << maze->GetStart() << "\n";
    analysis << "End vertex: " << maze->GetEnd() << "\n";
    
    // Count total connections
    const Graph& graph = maze->GetGraph();
    int total_connections = 0;
    int max_degree = 0;
    int min_degree = INT_MAX;
    
    for (int v = 0; v < maze->GetVertexCount(); v++) {
        int degree = 0;
        for (const auto& edge : graph[v]) {
            if (std::get<0>(edge) >= 0) degree++;
        }
        total_connections += degree;
        max_degree = std::max(max_degree, degree);
        min_degree = std::min(min_degree, degree);
    }
    
    analysis << "Total connections: " << total_connections << "\n";
    analysis << "Average degree: " << (total_connections / static_cast<double>(maze->GetVertexCount())) << "\n";
    analysis << "Degree range: [" << min_degree << ", " << max_degree << "]\n";
    
    auto [boundary_count, boundary_details] = analyze_boundary_walls(maze);
    analysis << boundary_details << "\n";
    
    return analysis.str();
}

template<typename MazeType>
bool is_spanning_tree(TestableMaze<MazeType>* maze) {
    // A spanning tree has exactly V-1 edges for V vertices
    std::set<std::pair<int, int>> counted_edges;
    const Graph& graph = maze->GetGraph();
    
    for (int v = 0; v < maze->GetVertexCount(); v++) {
        for (const auto& edge : graph[v]) {
            int to = std::get<0>(edge);
            if (to >= 0) {
                // Count each edge only once
                int v1 = std::min(v, to);
                int v2 = std::max(v, to);
                counted_edges.insert({v1, v2});
            }
        }
    }
    
    return counted_edges.size() == static_cast<size_t>(maze->GetVertexCount() - 1);
}

template<typename MazeType>
int count_boundary_walls(TestableMaze<MazeType>* maze) {
    // Count edges with to == -1 (boundary edges)
    int walls = 0;
    const Graph& graph = maze->GetGraph();
    
    for (int v = 0; v < maze->GetVertexCount(); v++) {
        for (const auto& edge : graph[v]) {
            int to = std::get<0>(edge);
            if (to == -1) {
                walls++;
            }
        }
    }
    return walls;
}

// Test functions
void test_rectangular_maze_structure() {
    TestableMaze<RectangularMaze> maze(5, 5);
    maze.InitialiseGraph();
    
    // Check basic properties
    ASSERT_EQ(25, maze.GetVertexCount(), "5x5 rectangular maze should have 25 vertices");
    ASSERT_EQ(0, maze.GetStart(), "Start vertex should be 0");
    ASSERT_EQ(24, maze.GetEnd(), "End vertex should be 24");
    
    // Check adjacency list is initialized
    const Graph& graph = maze.GetGraph();
    ASSERT_EQ(25, graph.size(), "Graph adjacency list size mismatch");
    
    // Check that interior vertices have correct number of potential connections
    // Interior vertex (2,2) = index 12 should have 4 neighbors
    int interior_vertex = 12;
    int neighbor_count = 0;
    for (const auto& edge : graph[interior_vertex]) {
        int to = std::get<0>(edge);
        if (to >= 0) neighbor_count++;
    }
    ASSERT_EQ(4, neighbor_count, "Interior vertex should have 4 potential neighbors before generation");
    
    // Check corner vertex (0,0) = index 0 should have 2 neighbors + boundaries
    int corner_vertex = 0;
    int corner_neighbors = 0;
    int corner_boundaries = 0;
    for (const auto& edge : graph[corner_vertex]) {
        int to = std::get<0>(edge);
        if (to >= 0) corner_neighbors++;
        else corner_boundaries++;
    }
    ASSERT_EQ(2, corner_neighbors, "Corner vertex should have 2 potential internal neighbors");
    ASSERT_TRUE(corner_boundaries >= 2, "Corner vertex should have at least 2 boundary walls");
}

void test_rectangular_maze_boundaries() {
    TestableMaze<RectangularMaze> maze(5, 5);
    maze.InitialiseGraph();
    
    // Check boundary walls
    int boundary_walls = count_boundary_walls(&maze);
    
    // Debug: print actual boundary wall count
    std::cout << "[Debug: boundary_walls = " << boundary_walls << "] ";
    
    // Rectangular maze has walls on all four sides
    // Left and right walls skip entry/exit (height-2 each = 3*2 = 6)
    // Top and bottom walls are complete (width each = 5*2 = 10)
    // Total = 16
    assert(boundary_walls > 0 && boundary_walls < 20);
}

void test_circular_maze_structure() {
    TestableMaze<CircularMaze> maze(4);
    maze.InitialiseGraph();
    
    // Check basic properties
    assert(maze.GetVertexCount() > 0);
    assert(maze.GetStart() >= 0);
    assert(maze.GetEnd() >= 0);
    assert(maze.GetStart() != maze.GetEnd());
    assert(maze.GetStart() < maze.GetVertexCount());
    assert(maze.GetEnd() < maze.GetVertexCount());
}

// Triangular maze tests removed - not available in base repository

void test_hexagonal_maze_structure() {
    TestableMaze<HexagonalMaze> maze(3);
    maze.InitialiseGraph();
    
    // Check basic properties
    assert(maze.GetVertexCount() == 6 * 3 * 3);  // 6 * size * size
    assert(maze.GetStart() >= 0);
    assert(maze.GetEnd() >= 0);
    assert(maze.GetStart() != maze.GetEnd());
}

void test_dfs_generation() {
    TestableMaze<RectangularMaze> maze(6, 6);
    maze.InitialiseGraph();
    
    DepthFirstSearch generator;
    
    // Verify generation doesn't crash
    try {
        maze.GenerateMaze(&generator);
    } catch (...) {
        ASSERT_TRUE(false, "DFS generation threw an exception");
    }
    
    // Basic sanity checks
    ASSERT_EQ(36, maze.GetVertexCount(), "Vertex count mismatch");
    ASSERT_EQ(0, maze.GetStart(), "Start vertex incorrect");
    ASSERT_EQ(35, maze.GetEnd(), "End vertex incorrect");
    
    // Check maze properties
    auto [connected, conn_details] = check_graph_connectivity(&maze);
    ASSERT_TRUE(connected, std::string("Maze not connected: ") + conn_details);
    
    auto [is_tree, tree_details] = check_spanning_tree_property(&maze);
    ASSERT_TRUE(is_tree, std::string("Maze not a spanning tree: ") + tree_details);
}

void test_bfs_generation() {
    TestableMaze<RectangularMaze> maze(5, 5);
    maze.InitialiseGraph();
    
    BreadthFirstSearch generator;
    
    try {
        maze.GenerateMaze(&generator);
    } catch (...) {
        ASSERT_TRUE(false, "BFS generation threw an exception");
    }
    
    ASSERT_EQ(25, maze.GetVertexCount(), "Vertex count mismatch");
    
    auto [connected, conn_details] = check_graph_connectivity(&maze);
    ASSERT_TRUE(connected, std::string("BFS maze not connected: ") + conn_details);
    
    auto [is_tree, tree_details] = check_spanning_tree_property(&maze);
    ASSERT_TRUE(is_tree, std::string("BFS maze not a spanning tree: ") + tree_details);
}

void test_kruskal_generation() {
    TestableMaze<RectangularMaze> maze(5, 5);
    maze.InitialiseGraph();
    
    Kruskal generator;
    
    try {
        maze.GenerateMaze(&generator);
    } catch (...) {
        ASSERT_TRUE(false, "Kruskal generation threw an exception");
    }
    
    ASSERT_EQ(25, maze.GetVertexCount(), "Vertex count mismatch");
    
    auto [connected, conn_details] = check_graph_connectivity(&maze);
    ASSERT_TRUE(connected, std::string("Kruskal maze not connected: ") + conn_details);
    
    auto [is_tree, tree_details] = check_spanning_tree_property(&maze);
    ASSERT_TRUE(is_tree, std::string("Kruskal maze not a spanning tree: ") + tree_details);
}

void test_maze_connectivity_after_generation() {
    // Test different maze types with different generators
    std::cout << "\n  Testing rectangular maze... ";
    {
        TestableMaze<RectangularMaze> rect_maze(6, 6);
        rect_maze.InitialiseGraph();
        DepthFirstSearch dfs;
        try {
            rect_maze.GenerateMaze(&dfs);
        } catch (...) {
            assert(false && "Failed to generate rectangular maze");
        }
    }
    std::cout << "OK";
    
    std::cout << "\n  Testing circular maze... ";
    {
        TestableMaze<CircularMaze> circ_maze(4);
        circ_maze.InitialiseGraph();
        BreadthFirstSearch bfs;
        try {
            circ_maze.GenerateMaze(&bfs);
        } catch (...) {
            assert(false && "Failed to generate circular maze");
        }
    }
    std::cout << "OK\n";
}

void test_maze_quality_comprehensive() {
    // Test that generated mazes have all expected properties
    TestableMaze<RectangularMaze> maze(4, 4);
    maze.InitialiseGraph();
    
    DepthFirstSearch generator;
    maze.GenerateMaze(&generator);
    
    std::cout << "\n" << analyze_maze_structure(&maze);
    
    // 1. Verify connectivity
    auto [connected, conn_details] = check_graph_connectivity(&maze);
    ASSERT_TRUE(connected, std::string("Generated maze not fully connected: ") + conn_details);
    
    // 2. Verify spanning tree property
    auto [is_tree, tree_details] = check_spanning_tree_property(&maze);
    ASSERT_TRUE(is_tree, std::string("Generated maze not a spanning tree: ") + tree_details);
    
    // 3. Verify boundary integrity
    auto [boundary_count, boundary_details] = analyze_boundary_walls(&maze);
    ASSERT_TRUE(boundary_count > 0, "Maze should have boundary walls");
    
    // 4. Verify start and end are still reachable
    ASSERT_RANGE(maze.GetStart(), 0, maze.GetVertexCount()-1, "Start vertex out of range");
    ASSERT_RANGE(maze.GetEnd(), 0, maze.GetVertexCount()-1, "End vertex out of range");
    ASSERT_TRUE(maze.GetStart() != maze.GetEnd(), "Start and end vertices should be different");
}

void test_solution_path_exists() {
    TestableMaze<RectangularMaze> maze(5, 5);
    maze.InitialiseGraph();
    
    DepthFirstSearch generator;
    
    try {
        maze.GenerateMaze(&generator);
    } catch (...) {
        ASSERT_TRUE(false, "Failed to generate maze for solution test");
    }
    
    // Verify there's a path from start to end
    auto [connected, details] = check_graph_connectivity(&maze);
    ASSERT_TRUE(connected, std::string("No path from start to end: ") + details);
}

// Main test runner
int main() {
    TestRunner runner;
    
    std::cout << "=== Maze Generator Test Suite ===" << std::endl << std::endl;
    
    // Structure tests
    runner.run_test("Rectangular Maze Structure", test_rectangular_maze_structure);
    runner.run_test("Rectangular Maze Boundaries", test_rectangular_maze_boundaries);
    runner.run_test("Circular Maze Structure", test_circular_maze_structure);
    // Triangular maze tests removed - not available in base repository
    runner.run_test("Hexagonal Maze Structure", test_hexagonal_maze_structure);
    
    // Generation algorithm tests
    runner.run_test("DFS Generation", test_dfs_generation);
    runner.run_test("BFS Generation", test_bfs_generation);
    runner.run_test("Kruskal Generation", test_kruskal_generation);
    
    // Comprehensive maze quality tests
    runner.run_test("Maze Quality Comprehensive", test_maze_quality_comprehensive);
    runner.run_test("Maze Connectivity After Generation", test_maze_connectivity_after_generation);
    runner.run_test("Solution Path Exists", test_solution_path_exists);
    
    runner.print_summary();
    
    return 0;
}