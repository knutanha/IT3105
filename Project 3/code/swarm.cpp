#include <iostream>
#include <fstream>
#include <math.h>
#include <limits>

#include "swarm.h"
#include "circleproblemparticle.h"
#include "particleknapsack.h"


using namespace std;


Swarm::Swarm(ProblemType type, int nDimensions, std::pair<double, double> searchSpace, std::pair<double, double> velocityDomain, int nParticles, int nNeighbours,
             double globalAttractionWeight, double localAttractionWeight, double velocityWeight, bool write)
{
    _nDimensions = nDimensions;
    _searchSpace = searchSpace;
    _nParticles = nParticles;
    _nNeighbours = nNeighbours;
    _iteration = 0;
    _currentBestScore = numeric_limits<float>::infinity();
    _write = write;
    _problemType = type;

    string filename;

    for (int i = 0; i < _nParticles; i++){
        switch (_problemType)
        {
        case CircleProblem:
            _particles.push_back(make_shared<CircleProblemParticle>(
                                 i,
                                 _nDimensions,
                                 _searchSpace,
                                 velocityDomain,
                                 globalAttractionWeight,
                                 localAttractionWeight,
                                 velocityWeight));
            break;
        case KnapSack1:
            filename = "pso-packages.txt";
            _knapsackLibrary = make_shared<KnapsackLibrary>(1000.0, 1000.0, filename, false);
            _particles.push_back(make_shared<ParticleKnapsack>(
                                 i,
                                 _nDimensions,
                                 _searchSpace,
                                 velocityDomain,
                                 globalAttractionWeight,
                                 localAttractionWeight,
                                 velocityWeight,
                                 _knapsackLibrary,
                                 true)
                                 );
            break;
        case KnapSack2:
            filename = "pso-packages.txt";
            _knapsackLibrary = make_shared<KnapsackLibrary>(1000.0, 1000.0, filename, true);
            _particles.push_back(make_shared<ParticleKnapsack>(
                                 i,
                                 _nDimensions,
                                 _searchSpace,
                                 velocityDomain,
                                 globalAttractionWeight,
                                 localAttractionWeight,
                                 velocityWeight,
                                 _knapsackLibrary,
                                 true)
                                 );
        default:
            break;
        }


        // Init current best result
        if (_particles[i]->getScore() <= _currentBestScore){
            _currentBestScore = _particles[i]->getScore();
            _bestKnownPosition = _particles[i]->getPosition();
        }

    }

}

void Swarm::iterateParticlePositions()
{
    for (auto particle : _particles){
        particle->iteratePosition();

        double particleScore = particle->getScore();


        // Update current best

        if (particleScore < _currentBestScore){
            _bestKnownPosition = particle->getPosition();
            _currentBestScore = particleScore;
        }
    }
}

void Swarm::generateParticleVelocities()
{
    // All particles see eachother

    vector<double> neighbourBestPos;
    double neighbourBestScore, temp;

    for (auto particle : _particles){
        // Find closest neighbours
        if (_nNeighbours > 0){
            neighbourBestScore = numeric_limits<float>::infinity();
            for (auto i : this->getNeighbours(particle->getId(), _nNeighbours))
            {
                temp = i->getBestScore();
                if (temp < neighbourBestScore){
                    neighbourBestScore = temp;
                    neighbourBestPos = i->getPosition();
                }
            }
        } else {
            neighbourBestPos = _bestKnownPosition;
        }
        particle->generateVelocity(_bestKnownPosition);
    }
}

void Swarm::iterateSwarm()
{
    this->iterateParticlePositions();
    this->generateParticleVelocities();
    _iteration++;
}

double Swarm::getGlobalFitness()
{
    double globalFitness = 0.0;
    switch(_problemType)
    {
    case CircleProblem:
        for (auto i : _particles){
            globalFitness += i->getBestScore();
        }
        break;
    case KnapSack1:
        // Step through
    case KnapSack2:
        globalFitness = numeric_limits<float>::infinity();
        break;
    default:
        break;
    }
    return globalFitness;
}

vector<shared_ptr<Particle> > Swarm::getNeighbours(int particleId, int n)
{
    int largestDistanceId;
    double distance, largestDistance;
    vector<shared_ptr<Particle> > neighbours;
    vector<double> distances, tempPos;

    shared_ptr<Particle> particle = _particles[particleId];
    int id = particle->getId();
    vector<double> pos = particle->getPosition();

    int counter = 0;



    // DEBUG
//    cout << "Main particle: pos(";
//    for (auto k : pos)
//        cout << k << " ";
//    cout << ")"  << endl;

    // Search through all particles to find neighbours
    for (auto i : _particles){
        if (i->getId() != id)
        {
            tempPos = i->getPosition();
            distance = euclidean_distance(pos, tempPos);
            if (counter == 0){
                neighbours.push_back(i);
                distances.push_back(distance);
                largestDistance = distance;
                largestDistanceId = 0;
            }
            else if (counter < n) {
                neighbours.push_back(i);
                distances.push_back(distance);
                if (largestDistance < distance){
                    largestDistance = distance;
                    largestDistanceId = neighbours.size()-1;
                }

            }
            else {
                if (distance < largestDistance){
                    // Remove the neighbour that is furthest away
                    neighbours.erase(neighbours.begin() + largestDistanceId);
                    distances.erase(distances.begin() + largestDistanceId);
                    // Add the new neighbour
                    neighbours.push_back(i);
                    distances.push_back(distance);
                    // Find which neighbour is furthest away now
                    largestDistanceId = 0;
                    largestDistance = distances[largestDistanceId];
                    for (int j = 1; j < n; j++)
                    {
                        if (distances[j] > largestDistance) {
                            largestDistance = distances[j];
                            largestDistanceId = j;
                        }
                    }
                }

            }
            counter++;

            if (neighbours.size() > n)
                cout << "Error! Something wrong in the neighbourhood!" << endl;
        }

    }
    // DEBUG
    //cout << "\nNeighbourhood:" << endl;
    //cout << "Largest distance(id = " << largestDistanceId << ") = " << largestDistance << endl;
    //for (int j = 0; j < neighbours.size(); j++)
    //{
    //    tempPos = neighbours[j]->getPosition();
    //    cout << j << ": pos(";
    //    for (auto k : tempPos)
    //        cout << k << " ";
    //    cout << "), distance(" << distances[j] << ")" << endl;
    //}

    return neighbours;
}

PSOResult Swarm::runPSO(int maxIt, double globalFitnessRequirement)
{
    cout << maxIt << endl;
    double globalFitness;

    //--- LOG ---//
    ofstream logfile;
    if (_write == true)
    {
        logfile.open ("log.txt");

        logfile << "it\tP\t";
        for (int i = 0; i < _nDimensions; i++)
        {
            logfile << "x" << i << "\t";
        }
        logfile << "score\tcurrentBest\n";
    }

    for (int i = 0; i < maxIt; i++)
    {

        this->iterateSwarm();
        globalFitness = this->getGlobalFitness();
        _globalFitnessHistory.push_back(globalFitness);
        if (globalFitness <= globalFitnessRequirement){
            cout << "Reached global fitness at " << _iteration << " iterations!" << endl;
            break;
        }

        //--- LOG ---//
        if (_write == true){
            for (int j = 0; j < _nParticles; j++){
                vector<double> pos = _particles[j]->getPosition();
                logfile << i << "\t" << j << "\t";
                for (int k = 0; k < _nDimensions; k++){
                    logfile << pos[k] << "\t";
                }
                logfile << _particles[j]->getScore() << "\t";
                logfile << _particles[j]->getBestScore() << endl;
            }
        }

    }

    //--- LOG ---//
    if (_write == true)
    {
        logfile.close();
        logfile.open ("log2.txt");
        for (auto i : _globalFitnessHistory)
            logfile << i << endl;
        logfile.close();
    }
    return PSOResult(_iteration,
                     maxIt,
                     globalFitness,
                     globalFitnessRequirement,
                     _currentBestScore,
                     _bestKnownPosition);


}

void Swarm::printState()
{
    cout << "\nSWARM:" << endl;
    cout << "Dimensions = " << _nDimensions << endl;
    cout << "Number of particles = (" << _nParticles <<  "," << _particles.size() << ")" << endl;
    cout << "it = " << _iteration << endl;
    cout << "bestScore = " << _currentBestScore << endl;
    cout << "bestPos = [";
    for (auto i : _bestKnownPosition)
        cout << i << " ";
    cout << "]\nPARTICLES:" << endl;
    for (auto i : _particles){
        i->printState();
    }

}

void Swarm::printParticle(int particleId)
{
    if (particleId < _particles.size() && particleId >= 0)
        _particles[particleId]->printState();
}

