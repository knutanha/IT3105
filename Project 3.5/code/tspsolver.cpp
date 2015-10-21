#include "tspsolver.h"

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

double neighbourhoodFunction(int d, double omega)
{
    double dd = static_cast<double>(d);
    return exp(- dd*dd / (omega*omega));
}

pair<double,double>
calculateWeight (pair<double,double> neuronWeight,
                 pair<double,double> cityWeight,
                 int neuronDistance, double omega, double alpha)
{



    return pair<double,double>(neuronWeight.first +
                               alpha * neighbourhoodFunction(neuronDistance, omega) *
                               (cityWeight.first - neuronWeight.first),
                               neuronWeight.second +
                               alpha * neighbourhoodFunction(neuronDistance, omega) *
                               (cityWeight.second - neuronWeight.second) );
}

double
euclideanDistance(pair<double,double> a, pair<double,double> b)
{
    double temp = (a.first - b.first)*(a.first - b.first);
    temp += (a.second - b.second)*(a.second - b.second);
    temp = sqrt(temp);
    return temp;
}


TSPSolver::TSPSolver(const string filename, int k, int maxit,
                     double alpha_0, double omega_0, double alpha_tau, double omega_tau)
{
    _cities = make_shared<PointSet>(filename);
    _neurons = make_shared<Neurons>(_cities->getDimension() * 2);

    _neighbourhoodCap = 0.40;
    _relativeChangeRequirement = 0.0001;
    _omega = omega_0;
    _alpha = alpha_0;
    _maxIterations = maxit;
    _ALPHA_TAU = alpha_tau;
    _OMEGA_TAU = omega_tau;
    _logInterval = k;

    // Write log file header
    stringstream ss;
    ss << "tsp_";
    ss << _cities->getDimension();
    ss << ".log";
    _logName = ss.str();
    fstream stream;
    cout << "Clearing log file!" << endl;
    stream.open(_logName, ios::out | ios::trunc);
    stream << "it\tindex\tx\ty" << endl;
    stream.close();
}

void TSPSolver::runAlgorithm()
{
    bool test = true;

    // Variable declarations
    int numberOfNeurons = _neurons->getDimension();
    int selectedNeuron, closestNeuron, index;
    double tempDistance, xdiff, ydiff, closestDistance,
           weightChange, pathLength, alpha, omega;
    pair<double,double> cityWeights, cityWeights2, neuronWeights,
                        oldWeight, newWeight;
    vector<int> path;
    shared_ptr<vector<int> > cityList;

    // Fills vector with false (neurons are not assigned to a city yet)
    vector<bool> neuronAssignmentStatus(numberOfNeurons);

    //if(test) _neurons->print();

    writeStatusToFile(0);

    omega = _omega;
    alpha = _alpha;

    for (int iteration = 1; iteration <= _maxIterations; iteration++)
    {
        // Going through all cities in random order
        cityList = _cities->getRandomOrder();
        for (int currentCity : *cityList)
        {

            /////////////////////////////////////////////////

                   // Finding the closest neuron //

            /////////////////////////////////////////////////

            // Gather information about the current city
            cityWeights = _cities->getWeights(currentCity);

            //DEBUG
            //if(test) cout << "City: (" << cityWeights.first << ", " << cityWeights.second << ")" << endl;


            // Reset previous calculations
            closestDistance = numeric_limits<double>::max();

            // Finding closest unassigned neuron
            for (int neuron = 0; neuron < numberOfNeurons; neuron++)
            {
                // Needs to be unassigned
                if (neuronAssignmentStatus[neuron] == false){

                    // Getting the neurons weights
                    neuronWeights = _neurons->getWeight(neuron);
                    xdiff = cityWeights.first - neuronWeights.first;
                    ydiff = cityWeights.second - neuronWeights.second;

                    // Calculating eucledian distance
                    tempDistance = sqrt(xdiff*xdiff + ydiff*ydiff);

                    if (tempDistance < closestDistance){
                        closestNeuron = neuron;
                        closestDistance = tempDistance;
                    }
                }
            }

            // Setting the selected neuron as assigned
            neuronAssignmentStatus[closestNeuron] = true;


            /////////////////////////////////////////////////

                 // Adjusting the weights of neurons //

            /////////////////////////////////////////////////

            // The neuron learns
            oldWeight = _neurons->getWeight(closestNeuron);
            newWeight = calculateWeight(oldWeight, cityWeights, 0, omega, alpha);
            weightChange = euclideanDistance(oldWeight, newWeight);
            _neurons->setWeight(closestNeuron, newWeight);

            //DEBUG
            //if (test) cout << "Closest neuron: " << closestNeuron << endl <<
            //                  "  Old Position = " << oldWeight.first << ", " << oldWeight.second << endl <<
            //                  "  New Position = " << newWeight.first << ", " << newWeight.second << endl;

            //DEBUG
            //if (test) cout << "Number of neighbours = " << static_cast<int>((numberOfNeurons * _neighbourhoodCap)/2)*2 << "of " << numberOfNeurons-1 << " possible!" << endl;

            // Propagating change to neighbouring nodes
            for (int i = 1; i < static_cast<int>((numberOfNeurons * _neighbourhoodCap)/2); i++)
            {

                // Negative direction (neuron topology)
                selectedNeuron = (closestNeuron - i >= 0) ?
                                 (closestNeuron - i) :
                                 (numberOfNeurons - (i - closestNeuron));
                oldWeight = _neurons->getWeight(selectedNeuron);
                newWeight = calculateWeight(oldWeight, cityWeights, i, omega, alpha);
                _neurons->setWeight(selectedNeuron, newWeight);

                // DEBUG
                //if(closestNeuron == 0)
                //    cout << "Negative: " << selectedNeuron << endl;

                // Positive direction
                selectedNeuron = (closestNeuron + i < numberOfNeurons) ?
                                 (closestNeuron + i) :
                                 (i - (numberOfNeurons - closestNeuron));
                oldWeight = _neurons->getWeight(selectedNeuron);
                newWeight = calculateWeight(oldWeight, cityWeights, i, omega, alpha);
                _neurons->setWeight(selectedNeuron, newWeight);
                // DEBUG
                //if(closestNeuron == 75)
                //    cout << "Positive: " << selectedNeuron << endl;

                // Stop if relative change becomes too low compared to the main neuron
                if (euclideanDistance(oldWeight,newWeight) < weightChange*_relativeChangeRequirement)
                    break;
            }

            //DEBUG
            //if(test) break;

        }
        if (iteration % _logInterval == 0)
        {
            writeStatusToFile(iteration);
            cout << "Iteration " << iteration << ": " << findPathLength(findPath()) << " units path length!" << endl;
        }

        cout << "alpha = " << alpha << endl;
        cout << "omega = " << omega << endl;

        /////////////////////////////////////////////////

              // Updating algorithm parameters //

        /////////////////////////////////////////////////

        alpha = _alpha * exp(-iteration / _ALPHA_TAU);
        omega = _omega * exp(-iteration / _OMEGA_TAU);
        //_alpha = 1.0/(pow(iteration,-1/4.0));
        //_omega = _omega * (1 - 0.01*iteration);


        fill(neuronAssignmentStatus.begin(), neuronAssignmentStatus.end(), false);

        //DEBUG
        //if (test) _neurons->print();

    }

    /////////////////////////////////////////////////

                 // Creating Path //

    /////////////////////////////////////////////////

    path = findPath();
    pathLength = findPathLength(path);

    // Write to file
    writePathToFile(_maxIterations+1, path);

    cout << "Path found was " << pathLength << " units long!" << endl;
}


void TSPSolver::printStatus()
{
    cout << "\n ----- TSP SOLVER STATUS -----\n" << endl;
    _cities->print();
    _neurons->print();
    cout << "\n ----- STATUS PRINT END -----\n" << endl;
}

void TSPSolver::writeStatusToFile(int iteration)
{
    pair<double,double> temp;
    ofstream out;
    out.open(_logName, ios::app);
    if (out.is_open())
    {
        for (int i = 0; i < _neurons->getDimension(); i++)
        {
            temp = _neurons->getWeight(i);
            out << iteration << "\t" << i << "\t" << temp.first << "\t" << temp.second << endl;
        }
        out.close();
    }
    else
    {
        cout << "Could not write to file!" << endl;
    }
}

void TSPSolver::writePathToFile(int iteration, vector<int> path)
{
    pair<double,double> temp;
    ofstream out;
    out.open(_logName, ios::app);
    if (out.is_open())
    {
        for (auto i : path)
        {
            temp = _cities->getWeights(i);
            out << iteration << "\t" << i << "\t" << temp.first << "\t" << temp.second << endl;
        }
        out.close();
    }
    else
    {
        cout << "Could not write to file!" << endl;
    }
}

std::vector<int> TSPSolver::findPath()
{
    int closestNeuron;
    int numberOfNeurons = _neurons->getDimension();
    int numberOfCities = _cities->getDimension();
    pair<double,double> neuronWeights, cityWeights;
    double xdiff, ydiff, tempDistance, closestDistance;
    vector<int> path;
    vector<int> neuronAssignmentStatus(numberOfNeurons);
    fill(neuronAssignmentStatus.begin(), neuronAssignmentStatus.end(),-1);


    for (int i = 0; i < numberOfCities; i++)
    {
        closestDistance = numeric_limits<double>::max();
        cityWeights = _cities->getWeights(i);
        for (int j = 0; j < numberOfNeurons; j++)
        {
            if (neuronAssignmentStatus[j] == -1)
            {
                neuronWeights = _neurons->getWeight(j);
                xdiff = cityWeights.first - neuronWeights.first;
                ydiff = cityWeights.second - neuronWeights.second;
                tempDistance = sqrt(xdiff*xdiff + ydiff*ydiff);

                if (tempDistance < closestDistance){
                    closestNeuron = j;
                    closestDistance = tempDistance;
                }
            }
        }
        neuronAssignmentStatus[closestNeuron] = i;
    }
    for (auto i : neuronAssignmentStatus){
        if (i != -1)
            path.push_back(i);
    }
    return path;
}

double TSPSolver::findPathLength(std::vector<int> path)
{
    double pathLength = 0.;
    pair<double,double> city1, city2;
    double tempDistance, xdiff, ydiff;
    int index;

    for (int i = 0; i < path.size(); i++){

        city1 = _cities->getWeights(path[i]);

        index = (i+1<path.size())?(i+1) : (0);
        city2 = _cities->getWeights(path[index]);
        xdiff = city1.first - city2.first;
        ydiff = city1.second - city2.second;

        tempDistance = sqrt(xdiff*xdiff + ydiff*ydiff);
        pathLength += tempDistance;
    }
    return pathLength;
}


