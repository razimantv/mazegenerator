# Maze generator

**Generate mazes of different shapes and arbitrary sizes**
![Maze types](/examples/mazes.png "Maze types")

Maze generator can create rectangular, hexagonal, honeycomb and circular mazes.
Maze generation can be done using Kruskal's algorithm, depth-first search or
breadth-first search. Mazes can be rendered in svg or png format (using gnuplot
as intermediate in the latter case).

## Dependencies

Maze generator uses gnuplot (with a system call `gnuplot`) to render png mazes.
So make sure that `gnuplot 5.0+` is installed with `pngcairo` terminal support
and is in the path if you wish to use png.

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
               [-t <output type] [-o <output prefix>]

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
  -w,-h   Width and height (rectangular maze)
  -t      Output type
          0: SVG output
          1: png output using gnuplot (.plt) intermediate
  -o      Prefix for .svg, .plt and .png outputs
  ```

## Issues

The arcs in the circular mazes are plotted as parametric curves in gnuplot, and
png can take quite long to render for large mazes.
