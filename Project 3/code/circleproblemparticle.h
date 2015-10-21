#ifndef CIRCLEPROBLEMPARTICLE_H
#define CIRCLEPROBLEMPARTICLE_H
#include "particle.h"
class CircleProblemParticle : public Particle
{
public:
    CircleProblemParticle(int id, int nDimensions,
                          std::pair<double,double> searchSpace,
                          std::pair<double, double> velocityDomain,
                          double globalAttractionWeight,
                          double localAttractionWeight,
                          double velocityWeight);
    double
    costFunction();

    void
    generateVelocity(const std::vector<double>& globalBestPosition);

    void
    iteratePosition();

    void
    printState();

    void
    printSummary();
};

#endif // CIRCLEPROBLEMPARTICLE_H
