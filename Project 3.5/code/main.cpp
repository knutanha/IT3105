#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <random>

#include "pointset.h"
#include "tspsolver.h"
#include "neurons.h"

using namespace std;

int main()
{
    srand(time(NULL));

    const string filename = "dj38.tsp";
    int k = 2;
    int maxit = 20;
    double alpha_0 = 1;
    double omega_0 = 20;
    double alpha_tau = 10;
    double omega_tau = 5;

    TSPSolver test(filename, k, maxit, alpha_0, omega_0, alpha_tau, omega_tau);
    test.runAlgorithm();

    return 0;
}

