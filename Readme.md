# Maze generator

**Generate mazes of different shapes and arbitrary sizes**
![Maze types](/examples/mazes.png "Maze types")

## Dependencies

Maze generator uses gnuplot (with a system call `gnuplot`) to plot the mazes.
So make sure that `gnuplot 5.0+` is installed with `pngcairo` terminal support
and is in the path.

The code is written in C++ 11, you will need a not-too-ancient C++ compiler to
build it.

## Installation

```
cd src; make
```

## Usage

```
Usage: mazegen [--help] [-m <maze type>] [-a <algorithm type>]
               [-s <size> | -w <width> -h <height>]
               [-o <output prefix>]

Optional arguments
  --help  Show this message and exit
  -m      Maze type
          0: Rectangular
          1: Hexagonal (triangular lattice)
          2: Honeycomb
          3: Circular
          4: Circular (triangular lattice)
  -a      Algorithm type
          0: Kruskal's algorithm
          1: Depth-first search
          2: Breadth-first search
  -s      Size (non-rectangular mazes)
  -w,-h   Width and height(rectangular maze)
  -o      Prefix for .plt and .png outputs
  ```

## Issues

The arcs in the circular mazes are plotted as parametric curves in gnuplot, and
can take quite long to render for large mazes.
