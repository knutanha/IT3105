#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include <memory>
#include <random>


class Particle {
public:
    Particle(int id,
             int nDimensions,
             std::pair<double,double> searchSpace,
             std::pair<double,double> velocityDomain,
             double globalAttractionWeight,
             double localAttractionWeight,
             double velocityWeight);
    // Virtuals
    void
    generateVelocity(const std::vector<double>& globalBestPosition);

    virtual void
    iteratePosition() = 0;

    virtual double
    costFunction() = 0;

    // GET
    std::vector<double>
    getPosition();

    std::vector<double>
    getVelocity();

    double
    getScore();

    int
    getIteration();

    double
    getBestScore();

    int
    getId();

    // REFRESH
    void
    updateScore();

    // IO
    virtual void
    printState() = 0;

    virtual void
    printSummary() = 0;

protected:
    int _id;

    std::pair<double,double> _searchSpace;
    std::pair<double,double> _velocityDomain;
    int _nDimensions;

    // Global Attraction Weight
    double _GAW;
    // Local Attraction Weight
    double _LAW;
    // Velocity Weight
    double _VW;

    int _iteration;
    int _scoreIteration;
    int _velocityIteration;

    std::vector<double> _position;
    std::vector<double> _velocity;
    double _score;

    double _currentBestScore;
    std::vector<double> _bestKnownPosition;
};

#endif // PARTICLE_H
