# Maze Generator Test Suite

This directory contains comprehensive tests for the maze generator library.

## Running Tests

To run the test suite:

```bash
cd tests
make clean
make
./run_tests
```

## Test Coverage

The test suite includes tests for:

### Maze Structure Tests
- **Rectangular Maze Structure**: Verifies correct vertex count, start/end positions, and adjacency list initialization
- **Rectangular Maze Boundaries**: Checks that boundary walls are correctly placed with entry/exit openings
- **Circular Maze Structure**: Validates basic properties of circular mazes
- **Triangular Maze Structure**: Ensures correct vertex count for triangular mazes
- **Triangular Maze Boundaries**: Verifies boundary wall placement
- **Hexagonal Maze Structure**: Tests hexagonal maze initialization

### Generation Algorithm Tests
- **DFS Generation**: Tests Depth-First Search maze generation
- **BFS Generation**: Tests Breadth-First Search maze generation
- **Kruskal Generation**: Tests Kruskal's algorithm for maze generation

### Integration Tests
- **Maze Connectivity After Generation**: Verifies that all maze types can be generated with different algorithms
- **Solution Path Exists**: Ensures mazes have valid paths from start to end

## Test Architecture

The tests use:
- Custom `TestRunner` class for organized test execution and reporting
- Template-based `TestableMaze` class to access protected members
- Assertion-based testing for immediate failure detection
- Graph connectivity and spanning tree validation algorithms

## Expected Output

A successful test run will show:
```
=== Maze Generator Test Suite ===

Running Rectangular Maze Structure... PASSED
Running Rectangular Maze Boundaries... PASSED
... (more tests) ...

=== Test Summary ===
Tests run: 11
Tests passed: 11
Tests failed: 0
Success rate: 100%
```

## Adding New Tests

To add a new test:
1. Create a test function following the pattern: `void test_feature_name()`
2. Use assertions to verify expected behavior
3. Add the test to the main function using: `runner.run_test("Test Name", test_feature_name)`
4. Recompile and run the test suite

## Known Issues

- Compiler warnings about virtual destructors in border classes are non-critical
- Tests focus on generation success rather than maze quality metrics