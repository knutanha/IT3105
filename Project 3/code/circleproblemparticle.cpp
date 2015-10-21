#include <iostream>

#include "circleproblemparticle.h"
#include "generaltools.h"

using namespace std;

CircleProblemParticle::CircleProblemParticle(int id,
                                             int nDimensions,
                                             std::pair<double,double> searchSpace,
                                             std::pair<double, double> velocityDomain,
                                             double globalAttractionWeight,
                                             double localAttractionWeight,
                                             double velocityWeight)
    : Particle(id, nDimensions, searchSpace, velocityDomain, globalAttractionWeight,
               localAttractionWeight, velocityWeight)
{
    for (int i = 0; i < nDimensions; i++){
        _position.push_back( rand_range(searchSpace));
        _bestKnownPosition.push_back(_position[i]);
    }
    this->updateScore();
    _currentBestScore = _score;
}

double CircleProblemParticle::costFunction()
{
    double cost = 0.0;

    for (auto i : _position){
        cost += (double)i*i;
    }

    return cost;
}



void CircleProblemParticle::iteratePosition()
{
    // Updating position
    if (_velocityIteration != _iteration){
        cout << "Error! Can\'t iterate before generating a new velocity!" << endl;
    } else {
        // New iteration
        _iteration++;

        for (int i = 0; i < _nDimensions; i++){
            _position[i] += _velocity[i];

            // Preventing particles from moving outside the domain
            if (_position[i] > _searchSpace.second){
                _position[i] = _searchSpace.second;
            } else if (_position[i] < _searchSpace.first) {
                _position[i] = _searchSpace.first;
            }
        }
        // Updating score
        this->updateScore();
        if (_score < _currentBestScore){
            _currentBestScore = _score;
            _bestKnownPosition = _position;
        }
        //cout << "status = (" << _position[0] << "," << _score << "," << _velocity[0] << ")" << endl;
    }
}
void CircleProblemParticle::printState()
{
    cout << "\nParticle:" << endl;
    cout << "Dimensions = " << _nDimensions << endl;
    cout << "GAW = " << _GAW << endl;
    cout << "LAW = " << _LAW << endl;
    cout << "VW = " << _VW << endl;
    cout << "it = " << _iteration << endl;
    cout << "scoreIt = " << _scoreIteration << endl;
    cout << "vIt = " << _velocityIteration << endl;
    cout << "score = " << _score << endl;
    cout << "pos = [";
    for (auto i : _position)
        cout << i << " ";
    cout << "]\nbestScore = " << _currentBestScore << endl;
    cout << "bestPos = [";
    for (auto i : _bestKnownPosition)
        cout << i << " ";
    cout << "]\nvelocity = [";
    for (auto i : _velocity)
        cout << i << " ";
    cout << "]" << endl;
}

void CircleProblemParticle::printSummary()
{
    cout << "\nParticle Summary:" << endl;
    cout << "Dimensions = " << _nDimensions << endl;
    cout << "GAW = " << _GAW << endl;
    cout << "LAW = " << _LAW << endl;
    cout << "VW = " << _VW << endl;
    cout << "it = " << _iteration << endl;
    cout << "scoreIt = " << _scoreIteration << endl;
    cout << "vIt = " << _velocityIteration << endl;
    cout << "score = " << _score << endl;
    cout << "bestScore = " << _currentBestScore << endl;
}
