#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include "pointset.h"
#include "neurons.h"

#include <memory>
#include <string>


class TSPSolver
{
public:

    TSPSolver(const std::string filename, int k, int maxit,
              double alpha_0, double omega_0, double alpha_tau, double omega_tau);

    void
    runAlgorithm();

    void
    printStatus();

    void
    writeStatusToFile(int iteration);

    std::vector<int>
    findPath();

    double
    findPathLength(std::vector<int> path);

    void
    writePathToFile(int iteration, std::vector<int> path);


private:
    std::shared_ptr<Neurons> _neurons;
    std::shared_ptr<PointSet> _cities;

    // Maximum number of iterations
    int _maxIterations;

    // Percentage of total number of neurons which are considered a neighbour
    // (To reduce calculations for very small contributions)
    double _neighbourhoodCap;
    double _relativeChangeRequirement;

    // Learning rate
    double _alpha;

    // Neighbourhood function variance (distribution of learning influence)
    double _omega;

    // Time constants
    double _ALPHA_TAU;
    double _OMEGA_TAU;

    // Log file name
    std::string _logName;

    int _logInterval;
};

#endif // TSPSOLVER_H
