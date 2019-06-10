all:
	g++ -std=c++17 -O3 -mtune=native -march=native -mfpmath=both -fopenmp main.cpp
