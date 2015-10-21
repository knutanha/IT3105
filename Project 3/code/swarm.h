#ifndef SWARM_H
#define SWARM_H

#include <vector>
#include <memory>

#include "particle.h"
#include "psoresult.h"
#include "knapsacklibrary.h"
#include "generaltools.h"

enum ProblemType{CircleProblem, KnapSack1, KnapSack2};

class Swarm
{
public:
    Swarm (ProblemType type, int nDimensions,
           std::pair<double,double> searchSpace,
           std::pair<double,double> velocityDomain,
           int nParticles,
           int nNeighbours,
           double globalAttractionWeight,
           double localAttractionWeight,
           double velocityWeight,
           bool write);

    void
    generateParticleVelocities();

    void
    iterateParticlePositions();

    void
    iterateSwarm();

    double
    getGlobalFitness();

    std::vector<std::shared_ptr<Particle> >
    getNeighbours(int particleId, int n);

    PSOResult
    runPSO(int maxIt, double globalFitnessRequirement);


    // IO
    void
    printState();

    void
    printParticle(int particleId);

private:
    int _iteration;
    int _nDimensions;
    int _nParticles;
    int _nNeighbours;
    bool _write;
    ProblemType _problemType;

    std::pair<double,double> _searchSpace;
    std::vector<double> _bestKnownPosition;
    double _currentBestScore;
    std::vector<std::shared_ptr<Particle> > _particles;

    std::shared_ptr<KnapsackLibrary> _knapsackLibrary;
    bool _cflExist;

    std::vector<double> _globalFitnessHistory;
};


#endif // SWARM_H
