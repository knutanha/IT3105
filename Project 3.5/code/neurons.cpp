#include "neurons.h"
#include <cmath>
#include <memory>
#include <random>
#include <iostream>

using namespace std;

Neurons::Neurons()
{
    _numberOfNeurons = 0;
}

Neurons::Neurons(int numberOfNeurons)
{

    // Some algorithm distributing initial coordinates of the neurons.
    pair<double,double> temp;
    _neurons = make_shared< vector<pair<double,double> > > ();
    for (int i = 0; i < numberOfNeurons; i++)
    {
        // Randomly distributed
        temp = pair<double,double>(rand()/(double)RAND_MAX, rand()/(double)RAND_MAX);
        _neurons->push_back(temp);
    }
    _numberOfNeurons = numberOfNeurons;
}

std::shared_ptr<std::vector<std::pair<double, double> > > Neurons::getWeights()
{
    return _neurons;
}

//int Neurons::getClosestNeuron(std::pair<double, double> coordinates)
//{
//    int currentSelected = 0;
//    double closestDistance = 2.0;
//    double tempDistance, xdiff, ydiff;

//    for (int i = 0; i < _neurons->size(); i++)
//    {
//        xdiff = coordinates.first - _neurons->at(i).first;
//        ydiff = coordinates.second - _neurons->at(i).second;
//        tempDistance = sqrt(xdiff*xdiff + ydiff*ydiff);

//        if (tempDistance < closestDistance){
//            currentSelected = i;
//            closestDistance = tempDistance;
//        }
//    }
//    return currentSelected;
//}

//std::vector<int> Neurons::getNeighbours(int neuron, int neighbourhoodRadius)
//{

//    if ( neighbourhoodRadius > (_numberOfNeurons-1)/2)
//        neighbourhoodRadius = (_numberOfNeurons-1)/2;

//    vector<int> output;
//    int negative, positive;

//    for (int i=1; i <= neighbourhoodRadius; i++)
//    {
//        negative = ( neuron-i >= 0) ? (neuron-i) : (_numberOfNeurons - ( i - neuron));
//        positive = ( neuron+i < _numberOfNeurons) ? ( neuron + i ) : (i - (_numberOfNeurons - neuron));
//        output.push_back(negative);
//        output.push_back(positive);
//    }
//    return output;
//}

std::pair<double, double> Neurons::getWeight(int neuron)
{
    if (neuron < _numberOfNeurons && neuron >= 0)
    {
        return _neurons->at(neuron);
    }
    else
    {
        cout << "Error(Neurons::getWeight): " << neuron << " is not a valid neuron!" << endl;
        return _neurons->at(0);
    }
}

int Neurons::getDimension()
{
    return _numberOfNeurons;
}

void Neurons::setWeight(int neuron, double x, double y)
{
    if (x >= 0 && x <= 1.f && y >= 0 && y <= 1.f)
    {
        if (neuron < _numberOfNeurons && neuron >= 0)
        {
            _neurons->at(neuron) = pair<double,double>(x,y);
        }
        else
        {
            cout << "Error(Neurons::setWeight): " << neuron << " is not a valid neuron!" << endl;
        }
    }
    else
    {
        cout << "Error(Neurons::setWeight): (" << x << "," << y << ") is outside the domain!" << endl;
    }
}

void Neurons::setWeight(int neuron, pair<double, double> weight)
{
    if (weight.first >= 0 && weight.first <= 1.f &&
        weight.second >= 0 && weight.second <= 1.f)
    {
        if (neuron < _numberOfNeurons && neuron >= 0)
        {
            _neurons->at(neuron) = weight;
        }
        else
        {
            cout << "Error(Neurons::setWeight): " << neuron << " is not a valid neuron!" << endl;
        }
    }
    else
    {
        cout << "Error(Neurons::setWeight): (" << weight.first << "," <<
                weight.second << ") is outside the domain!" << endl;
    }

}

void Neurons::print()
{
    cout << "NEURON PRINT!" << endl;
    cout << "Dimension = " << _numberOfNeurons << endl;

    for (int i = 0; i < _numberOfNeurons; i++)
    {
        cout << "(" << i << "): \t" << _neurons->at(i).first << "\t" << _neurons->at(i).second << endl;
    }
}

