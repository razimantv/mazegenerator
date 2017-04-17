#!/bin/bash

../src/mazegen -m 0 -w 40 -h 40 -a 2 -o -t 1 rectangular_BFS
../src/mazegen -m 1 -s 20 -a 0 -o -t 0 hexagonal_Kruskal
../src/mazegen -m 2 -s 20 -a 1 -o -t 1 honeycomb_DFS
../src/mazegen -m 3 -s 20 -a 1 -o -t 0 circular_DFS
../src/mazegen -m 4 -s 20 -a 0 -o circularhexagon_Kruskal

