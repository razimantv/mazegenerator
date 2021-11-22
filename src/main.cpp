#include "breadthfirstsearch.h"
#include "circularhexagonmaze.h"
#include "circularmaze.h"
#include "depthfirstsearch.h"
#include "hexagonalmaze.h"
#include "honeycombmaze.h"
#include "kruskal.h"
#include "looperasedrandomwalk.h"
#include "prim.h"
#include "rectangularmaze.h"
#include "usermaze.h"
#include <cstring>
#include <iostream>
#include <map>
#include <string>

void usage(std::ostream &out) {
  out << "Usage: mazegen [--help] [-m <maze type>] [-a <algorithm type>]"
      << std::endl;
  out << "               [-s <size> | -w <width> -h <height>]" << std::endl;
  out << "               [-t <output type>] [-o <output prefix>]" << std::endl;
  out << "               [-f <graph description file (for m=5)>]" << std::endl;

  out << std::endl;
  out << "Optional arguments" << std::endl;
  out << "  --help  "
      << "Show this message and exit" << std::endl;
  out << "  -m      "
      << "Maze type" << std::endl;
  out << "          "
      << "0: Rectangular (default)" << std::endl;
  out << "          "
      << "1: Hexagonal (triangular lattice)" << std::endl;
  out << "          "
      << "2: Honeycomb" << std::endl;
  out << "          "
      << "3: Circular" << std::endl;
  out << "          "
      << "4: Circular (triangular lattice)" << std::endl;
  out << "          "
      << "5: User defined graph" << std::endl;
  out << "  -a      "
      << "Algorithm type" << std::endl;
  out << "          "
      << "0: Kruskal's algorithm (default)" << std::endl;
  out << "          "
      << "1: Depth-first search" << std::endl;
  out << "          "
      << "2: Breadth-first search" << std::endl;
  out << "          "
      << "3: Loop-erased random walk" << std::endl;
  out << "          "
      << "4: Prim's algorithm" << std::endl;
  out << "  -s      "
      << "Size (non-rectangular mazes, default: 20)" << std::endl;
  out << "  -w,-h   "
      << "Width and height (rectangular maze, default: 20)" << std::endl;
  out << "  -t      "
      << "Output type" << std::endl;
  out << "          "
      << "0: svg output (default)" << std::endl;
  out << "          "
      << "1: png output using gnuplot (.plt) intermediate " << std::endl;
  out << "  -o      "
      << "Prefix for .svg, .plt and .png outputs (default: maze)" << std::endl;
  out << "  -c      "
      << "The color of the lines of the maze (defaut: black)" << std::endl;
  out << "  -b      "
      << "The color of the background of the maze (defaut: white)" << std::endl;
  out << "  -l      "
      << "The width of the lines of the maze (default: 3)" << std::endl;
}

int main(int argc, char *argv[]) {
  std::string outputprefix = "maze", infile = "";
  std::string color = "black", backColor = "white";
  int strokeWidth = 3;
  std::map<std::string, int> optionmap{{"-m", 0},  {"-a", 0},     {"-s", 20},
                                       {"-w", 20}, {"-h", 20},    {"-o", 0},
                                       {"-f", 0},  {"--help", 0}, {"-t", 0},
                                       {"-c", 0},  {"-l", 0},     {"-b", 0}};

  for (int i = 1; i < argc; i++) {
    if (optionmap.find(argv[i]) == optionmap.end()) {
      std::cerr << "Unknown argument " << argv[i] << "\n";
      usage(std::cerr);
      return 1;
    }
    
    if (strcmp("-o", argv[i]) == 0) {
      if (i + 1 == argc) {
        std::cerr << "Missing output prefix" << std::endl;
        usage(std::cerr);
        return 1;
      }
      outputprefix = argv[++i];
      continue;
    } else if (strcmp("-f", argv[i]) == 0) {
      if (i + 1 == argc) {
        std::cerr << "Missing maze input file" << std::endl;
        usage(std::cerr);
        return 1;
      }
      infile = argv[++i];
      continue;
    } else if (strcmp("--help", argv[i]) == 0) {
      usage(std::cout);
      return 0;
    }

    if (i + 1 == argc) {
      std::cerr << "Missing option for argument " << argv[i] << std::endl;
      usage(std::cerr);
      return 1;
    }

    if (strcmp("-c", argv[i]) == 0) {
      std::cout << "hi\n";
      color = argv[++i];
    } else if (strcmp("-l", argv[i]) == 0) {
      strokeWidth = std::stoi(argv[++i]);
    } else if (strcmp("-b", argv[i]) == 0) {
      backColor = argv[++i];
    } else {
      int x;
      try {
        x = std::stoi(argv[i + 1]);
      } catch (...) {
       std::cerr << "Invalid argument " << argv[i + 1] << " for option "
                 << argv[i] << "\n";
       usage(std::cerr);
       return 0;
      }
      optionmap[argv[i++]] = x;
    }
  }

  Maze *maze;
  SpanningtreeAlgorithm *algorithm;

  switch (optionmap["-m"]) {
    case 0:
      if (optionmap["-w"] < 1 or optionmap["-h"] < 1) {
        std::cerr << "Invalide size " << optionmap["-w"] << "x"
                  << optionmap["-h"] << " for rectangular maze\n";
        usage(std::cerr);
        return 1;
      }
      std::cout << "Rectangular maze of size " << optionmap["-w"] << "x"
                << optionmap["-h"] << "\n";
      maze = new RectangularMaze(optionmap["-w"], optionmap["-h"]);
      break;

    case 1:
      if (optionmap["-s"] < 1) {
        std::cerr << "Invalide size " << optionmap["-s"]
                  << " for hexagonal maze with triangular lattice\n";
        usage(std::cerr);
        return 1;
      }
      std::cout << "Hexagonal maze with triangular lattice of size "
                << optionmap["-s"] << "\n";
      maze = new HexagonalMaze(optionmap["-s"]);
      break;

    case 2:
      if (optionmap["-s"] < 1) {
        std::cerr << "Invalide size " << optionmap["-s"]
                  << " for honeycomb maze\n";
        usage(std::cerr);
        return 1;
      }
      std::cout << "Honeycomb maze of size " << optionmap["-s"] << "\n";
      maze = new HoneyCombMaze(optionmap["-s"]);
      break;

    case 3:
      if (optionmap["-s"] < 1) {
        std::cerr << "Invalide size " << optionmap["-s"]
                  << " for circular maze\n";
        usage(std::cerr);
        return 1;
      }
      std::cout << "Circular maze of size " << optionmap["-s"] << "\n";
      maze = new CircularMaze(optionmap["-s"]);
      break;

    case 4:
      if (optionmap["-s"] < 1) {
        std::cerr << "Invalide size " << optionmap["-s"]
                  << " for circular maze with triangular lattice\n";
        usage(std::cerr);
        return 1;
      }
      std::cout << "Circular maze with triangular lattice of size "
                << optionmap["-s"] << "\n";
      maze = new CircularHexagonMaze(optionmap["-s"]);
      break;

    case 5:
      if (infile == "") {
        std::cerr
            << "Graph description file not provided for user-defined graph\n";
        usage(std::cerr);
        return 1;
      }
      std::cout << "User-defined graph\n";
      maze = new UserMaze(infile);
      break;

    default:
      std::cerr << "Unknown maze type " << optionmap["-m"];
      usage(std::cerr);
      return 1;
  }

  switch (optionmap["-a"]) {
    case 0:
      std::cout << "Maze generation using Kruskal's algorithm\n";
      algorithm = new Kruskal;
      break;

    case 1:
      std::cout << "Maze generation using Depth-first search\n";
      algorithm = new DepthFirstSearch;
      break;

    case 2:
      std::cout << "Maze generation using Breadth-first search\n";
      algorithm = new BreadthFirstSearch;
      break;

    case 3:
      std::cout << "Maze generation using Loop-erased random walk\n";
      algorithm = new LoopErasedRandomWalk;
      break;

    case 4:
      std::cout << "Maze generation using Prim's algorithm\n";
      algorithm = new Prim;
      break;

    default:
      std::cerr << "Unknown algorithm type " << optionmap["-a"];
      usage(std::cerr);
      return 1;
  }

  if (optionmap["-t"] < 0 or optionmap["-t"] > 1) {
    std::cerr << "Unknown output type " << optionmap["-a"];
    usage(std::cerr);
    return 1;
  }

  Style::init({
    color,
    backColor,
    strokeWidth 
  });

  std::cout << "Initialising graph..." << std::endl;
  maze->InitialiseGraph();
  std::cout << "Generating maze..." << std::endl;
  maze->GenerateMaze(algorithm);
  if (optionmap["-t"] == 0) {
    std::cout << "Rendering maze to '" << outputprefix << ".svg'..."
              << std::endl;
    maze->PrintMazeSVG(outputprefix);
  } else {
    std::cout << "Exporting maze plotting parameters to '" << outputprefix
              << ".plt' ..." << std::endl;
    maze->PrintMazeGnuplot(outputprefix);
    std::cout << "Rendering maze to '" << outputprefix
              << ".png' using gnuplot..." << std::endl;
    system(("gnuplot '" + outputprefix + ".plt'").c_str());
  }
  return 0;
}
