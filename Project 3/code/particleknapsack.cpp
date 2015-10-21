#include <iostream>

#include "particleknapsack.h"
#include "generaltools.h"

using namespace std;

ParticleKnapsack::ParticleKnapsack(int id,
                                   int nDimensions,
                                   std::pair<double, double> searchSpace,
                                   std::pair<double, double> velocityDomain,
                                   double globalAttractionWeight,
                                   double localAttractionWeight,
                                   double velocityWeight,
                                   shared_ptr<KnapsackLibrary> cfl,
                                   bool randomize)
    : Particle(id, nDimensions, searchSpace, velocityDomain, globalAttractionWeight,
               localAttractionWeight, velocityWeight)
{
    int avgObjects, maxObjects, minObjects, objectVariance, numberOfPackages;
    statistics weightStats;
    vector<int> selectedPackages;

    _position.resize (nDimensions, searchSpace.first);
    _knapsackLibrary = cfl;
    _randomize = randomize;


    // Select start packages
    weightStats = _knapsackLibrary->getWeightStats();
    avgObjects = (int)_knapsackLibrary->getWeightCapacity()/weightStats.average;
    maxObjects = (int)_knapsackLibrary->getWeightCapacity()/weightStats.minimum;
    minObjects = (int)_knapsackLibrary->getWeightCapacity()/weightStats.maximum;
    //cout << "avgObjects" << avgObjects << endl;
    //cout << "maxObjects" << maxObjects << endl;
    //cout << "minObjects" << minObjects << endl;

    objectVariance = abs(avgObjects - maxObjects) < abs(avgObjects - minObjects)
                        ? abs(avgObjects - maxObjects) : abs(avgObjects - minObjects);
    objectVariance = objectVariance/2;
    //cout << "objectVariance = " << objectVariance << endl;

    numberOfPackages = rand_range(avgObjects - objectVariance, avgObjects + objectVariance);

    //cout << "numberOfPackages = " << numberOfPackages << endl;


    if(numberOfPackages > nDimensions)
    {
        fill(_position.begin(), _position.end(),searchSpace.second);
    } else {
        selectedPackages = unique_randoms(0,nDimensions-1,numberOfPackages);
        for (auto i : selectedPackages)
        {
            _position[i] = searchSpace.second;
        }
    }
    this->updateScore();
    _bestKnownPosition = _position;
    _currentBestScore = _score;

    // Will randomize if criterias are fullfilled
    this->randomizePosition();

}

double ParticleKnapsack::costFunction()
{
    double score = 0.0;
    double weight = 0.0;
    double volume = 0.0;
    for (int i = 0; i < _nDimensions; i++)
    {
        score  -= _position[i] * _knapsackLibrary->getValue(i);
        weight += _position[i] * _knapsackLibrary->getWeight(i);
        volume += _position[i] * _knapsackLibrary->getVolume(i);
    }

    // Setting score really high if out of requrements
    if (volume > _knapsackLibrary->getVolumeCapacity() ||
        weight > _knapsackLibrary->getWeightCapacity()) {
        score = -score;
    }

    _currentWeight = weight;
    _currentVolume = volume;

    return score;
}

void ParticleKnapsack::iteratePosition()
{
    double sigmoidVelocity;
    // Updating position
    if (_velocityIteration != _iteration){
        cout << "Error! Can\'t iterate before generating a new velocity!" << endl;
    } else {
        // New iteration
        _iteration++;

        for (int i = 0; i < _nDimensions; i++){
            sigmoidVelocity = sigmoid_map(_velocity[i]);
            _position[i] = rand_range(0.0,1.0) < sigmoidVelocity ? _searchSpace.second:_searchSpace.first;
        }
        // Updating score
        this->updateScore();
        if (_score < _currentBestScore){
            _currentBestScore = _score;
            _bestKnownPosition = _position;
        }

        // Will randomize if criterias are fullfilled
        this->randomizePosition();
    }
}

void ParticleKnapsack::printState()
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
    cout << "weight = " << _currentWeight << endl;
    cout << "volume = " << _currentVolume << endl;
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

void ParticleKnapsack::printSummary()
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
    cout << "weight = " << _currentWeight << endl;
    cout << "volume = " << _currentVolume << endl;
    cout << "bestScore = " << _currentBestScore << endl;
}

void ParticleKnapsack::randomizePosition()
{
    if (_randomize == true &&
        (_currentWeight > _knapsackLibrary->getWeightCapacity() ||
        _currentVolume > _knapsackLibrary->getVolumeCapacity()))
    {
        int avgObjects, maxObjects, minObjects, objectVariance, numberOfPackages;
        statistics weightStats;


        // Select start packages
        weightStats = _knapsackLibrary->getWeightStats();
        avgObjects = (int)_knapsackLibrary->getWeightCapacity()/weightStats.average;
        maxObjects = (int)_knapsackLibrary->getWeightCapacity()/weightStats.minimum;
        minObjects = (int)_knapsackLibrary->getWeightCapacity()/weightStats.maximum;

        objectVariance = abs(avgObjects - maxObjects) < abs(avgObjects - minObjects)
                            ? abs(avgObjects - maxObjects) : abs(avgObjects - minObjects);
        objectVariance = objectVariance/2;
        numberOfPackages = rand_range(avgObjects - objectVariance, avgObjects + objectVariance);

        if(numberOfPackages > _nDimensions)
        {
            numberOfPackages = _nDimensions/2;
        }

        int counter = 0;
        while ((_currentWeight > _knapsackLibrary->getWeightCapacity() ||
                _currentVolume > _knapsackLibrary->getVolumeCapacity()) &&
                _randomize==true)
        {

            fill(_position.begin(), _position.end(),_searchSpace.first);
            vector<int> selectedPackages = unique_randoms(0,_nDimensions-1,numberOfPackages);
            for (auto i : selectedPackages)
            {
                _position[i] = _searchSpace.second;
            }
            counter++;
            numberOfPackages--;
            this->updateScore();
        }
    }
}
