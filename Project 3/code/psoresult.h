#ifndef PSORESULT_H
#define PSORESULT_H

#include <vector>

class PSOResult
{
public:
    PSOResult(int it,
              int maxit,
              double globfit,
              double globfitreq,
              double score,
              std::vector<double> position);
    void
    printResult();

private:
    int _iterations;
    int _maxIt;
    double _globalFitness;
    double _globalFitnessRequirement;
    double _score;
    std::vector<double> _position;
};

#endif // PSORESULT_H
