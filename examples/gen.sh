#!/bin/bash

../src/mazegen -m 0 -w 40 -h 40 -a 2 -t 1 -o rectangular_BFS
../src/mazegen -m 1 -s 20 -a 0 -t 0 -o hexagonal_Kruskal
../src/mazegen -m 2 -s 20 -a 3 -t 1 -o honeycomb_LERW
../src/mazegen -m 3 -s 20 -a 1 -t 0 -o circular_DFS
../src/mazegen -m 4 -s 20 -a 0 -o circularhexagon_Kruskal
../src/mazegen -m 5 -a 4 -f manual_input.xy -o manual_Prim

