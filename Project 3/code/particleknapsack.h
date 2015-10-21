#ifndef PARTICLEKNAPSACK_H
#define PARTICLEKNAPSACK_H

#include "knapsacklibrary.h"
#include "particlediscrete.h"

class ParticleKnapsack : public Particle
{
public:
    ParticleKnapsack(int id,
                     int nDimensions,
                     std::pair<double,double> searchSpace,
                     std::pair<double,double> velocityDomain,
                     double globalAttractionWeight,
                     double localAttractionWeight,
                     double velocityWeight,
                     std::shared_ptr<KnapsackLibrary> cfl,
                     bool randomize);

    double
    costFunction();

    void
    iteratePosition();

    void
    printState();

    void
    printSummary();

    void
    randomizePosition();

protected:
    std::shared_ptr<KnapsackLibrary> _knapsackLibrary;
    double _currentWeight;
    double _currentVolume;
    bool _randomize;
};

#endif // PARTICLEKNAPSACK_H
