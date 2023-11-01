#include "maze.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include "depthfirstsearch.h"

Maze::Maze(int vertices, int startvertex, int endvertex)
    : vertices_(vertices), startvertex_(startvertex), endvertex_(endvertex) {}

void Maze::InitialiseGraph() {
  adjacencylist_.clear();
  adjacencylist_.resize(vertices_);
}

void Maze::GenerateMaze(SpanningtreeAlgorithm* algorithm) {
  auto spanningtree = algorithm->SpanningTree(vertices_, adjacencylist_);
  Solve(spanningtree);
  RemoveBorders(spanningtree);
}

void Maze::Solve(const std::vector<std::pair<int, int>>& edges) {
  Graph spanningtreegraph(vertices_);
  for (const auto& [u, v] : edges) {
    spanningtreegraph[u].push_back(
        *std::find_if(adjacencylist_[u].begin(), adjacencylist_[u].end(),
                      [v = v](const Edge& e) { return std::get<0>(e) == v; }));
    spanningtreegraph[v].push_back(
        *std::find_if(adjacencylist_[v].begin(), adjacencylist_[v].end(),
                      [u = u](const Edge& e) { return std::get<0>(e) == u; }));
  }

  DepthFirstSearch D;
  auto parent = D.Solve(vertices_, spanningtreegraph, startvertex_);
  solution_ = Graph(vertices_);
  for(int u = endvertex_; parent[u]!=u; u=parent[u]) {
    solution_[u].push_back(*std::find_if(
        spanningtreegraph[u].begin(), spanningtreegraph[u].end(),
        [u, &parent](const Edge& e) { return std::get<0>(e) == parent[u]; }));
  }
}

void Maze::RemoveBorders(const std::vector<std::pair<int, int>>& edges) {
  for (const auto& [u, v] : edges) {
    adjacencylist_[u].erase(
        std::find_if(adjacencylist_[u].begin(), adjacencylist_[u].end(),
                     [v = v](const Edge& e) { return std::get<0>(e) == v; }));
    adjacencylist_[v].erase(
        std::find_if(adjacencylist_[v].begin(), adjacencylist_[v].end(),
                     [u = u](const Edge& e) { return std::get<0>(e) == u; }));
  }
}

void Maze::PrintMazeGnuplot(const std::string& outputprefix, bool solution) const {
  std::ofstream gnuplotfile(outputprefix + ".plt");
  if (!gnuplotfile) {
    std::cerr << "Error opening " << outputprefix << ".plt for writing.\n";
    std::cerr << "Terminating.";
    exit(1);
  }

  gnuplotfile << "unset border\n";
  gnuplotfile << "unset tics\n";
  gnuplotfile << "set samples 15\n";
  gnuplotfile << "set lmargin at screen 0\n";
  gnuplotfile << "set rmargin at screen 1\n";
  gnuplotfile << "set bmargin at screen 0\n";
  gnuplotfile << "set tmargin at screen 1\n";

  double xmin, ymin, xmax, ymax;
  std::tie(xmin, ymin, xmax, ymax) = GetCoordinateBounds();
  gnuplotfile << "set xrange[" << xmin - 1 << ":" << xmax + 1 << "]\n";
  gnuplotfile << "set yrange[" << ymin - 1 << ":" << ymax + 1 << "]\n";

  int xresolution = int((xmax - xmin + 2) * 30),
      yresolution = int((ymax - ymin + 2) * 30);
  gnuplotfile << "set term pngcairo mono enhanced size " << xresolution << ","
              << yresolution << "\n";

  gnuplotfile << "set output '" << outputprefix << ".png'\n";
  gnuplotfile << "set multiplot\n";
  for (int i = 0; i < vertices_; ++i) {
    for (const auto& edge : adjacencylist_[i]) {
      if (std::get<0>(edge) < i)
        gnuplotfile << std::get<1>(edge)->GnuplotPrintString("black") << "\n";
    }
  }

  gnuplotfile << "plot 1/0 notitle\n";
  gnuplotfile << "unset multiplot\n";
  gnuplotfile << "set output\n";
}

void Maze::PrintMazeSVG(const std::string& outputprefix, bool solution) const {
  std::ofstream svgfile(outputprefix + ".svg");
  if (!svgfile) {
    std::cerr << "Error opening " << outputprefix << ".svg for writing.\n";
    std::cerr << "Terminating.";
    exit(1);
  }
  double xmin, ymin, xmax, ymax;
  std::tie(xmin, ymin, xmax, ymax) = GetCoordinateBounds();
  int xresolution = int((xmax - xmin + 2) * 30),
      yresolution = int((ymax - ymin + 2) * 30);

  svgfile << "<svg width=\"" << xresolution << "\" height=\"" << yresolution
          << "\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
  svgfile << "<g transform=\"translate(" << (1 - xmin) * 30 << ","
          << yresolution - (1 - ymin) * 30 << ") scale(1,-1)\">" << std::endl;
  svgfile << "<rect x=\"" << (xmin - 1) * 30 << "\" y=\"" << (ymin - 1) * 30
          << "\" width=\"" << xresolution << "\" height=\"" << yresolution
          << "\" fill=\"white\"/>" << std::endl;

  for (int i = 0; i < vertices_; ++i) {
    for (const auto& edge : adjacencylist_[i]) {
      if (std::get<0>(edge) < i) {
        svgfile << std::get<1>(edge)->SVGPrintString("black") << "\n";
      }
    }
  }
  svgfile << "</g>" << std::endl;
  svgfile << "</svg>" << std::endl;
}
