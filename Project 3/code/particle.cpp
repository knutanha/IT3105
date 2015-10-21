#include "particle.h"

#include <iostream>
#include <random>
#include <vector>
#include <math.h>

#include "generaltools.h"

using namespace std;

Particle::Particle(int id, int nDimensions,
                   pair<double, double> searchSpace,
                   pair<double, double> velocityDomain,
                   double globalAttractionWeight,
                   double localAttractionWeight,
                   double velocityWeight)
{
    _id = id;
    _iteration = 0;
    _searchSpace = searchSpace;
    _velocityDomain = velocityDomain;

    _nDimensions = nDimensions;

    // Preallocating memory
    _position.reserve(nDimensions);
    _velocity.reserve(nDimensions);
    _bestKnownPosition.reserve(nDimensions);

    _GAW = globalAttractionWeight;
    _LAW = localAttractionWeight;
    _VW = velocityWeight;

    for (int i = 0; i < nDimensions; i++){
        _velocity.push_back( rand_range(_velocityDomain));
    }
    _velocityIteration = _iteration;
}

void Particle::generateVelocity(const std::vector<double> &globalBestPosition)
{
    if (_iteration == _velocityIteration+1){
        _velocityIteration++;
    } else {
        cout << "Warning! Had already generated a velocity, generating a new one!" << endl;
        _velocityIteration = _iteration;
    }

    double tempVelocity;
    // Generating new velocity
    for (int i = 0; i < _nDimensions; i++){
        tempVelocity = _VW * _velocity[i] +
                       _LAW * rand_range(0.0,1.0) * (_bestKnownPosition[i] - _position[i]) +
                       _GAW * rand_range(0.0,1.0) * (globalBestPosition[i] - _position[i]);
        // Clapming velocity
        tempVelocity = tempVelocity>_velocityDomain.second?_velocityDomain.second:tempVelocity;
        tempVelocity = tempVelocity<_velocityDomain.first?_velocityDomain.first:tempVelocity;
        _velocity[i] = tempVelocity;
    }

}



std::vector<double> Particle::getPosition()
{
    return _position;
}

std::vector<double> Particle::getVelocity()
{
    return _velocity;
}

double Particle::getScore()
{
    if (_scoreIteration != _iteration)
        this->updateScore();
    return _score;
}

int Particle::getIteration()
{
    return _iteration;
}

void Particle::updateScore()
{
    _score = this->costFunction();

    if (_score < _currentBestScore)
    {
        _currentBestScore = _score;
        _bestKnownPosition = _position;
    }
    _scoreIteration = _iteration;
}

double Particle::getBestScore()
{
    return _currentBestScore;
}

int Particle::getId()
{
    return _id;
}





