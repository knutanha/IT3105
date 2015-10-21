#include "knapsacklibrary.h"
#include "generaltools.h"

#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

KnapsackLibrary::KnapsackLibrary(double weight,
                                 double volume,
                                 const string filename,
                                 bool option)
{
    double temp;
    ifstream input;
    input.open(filename.c_str());
    _weight = weight;
    _volume = volume;

    _objValues  = make_shared <vector<double> >();
    _objWeights = make_shared <vector<double> >();
    _objVolumes  = make_shared <vector<double> >();

    _weightStats.average = 0.0;
    _weightStats.maximum = 0.0;
    _weightStats.minimum = numeric_limits<float>::infinity();

    _volumeStats.average = 0.0;
    _volumeStats.maximum = 0.0;
    _volumeStats.minimum = numeric_limits<float>::infinity();

    // Collecting data from txt file
    if(input.is_open()){
        while (input >> temp)
        {
            // Value
            _objValues->push_back(temp);
            input >> temp;

            // Weight
            _objWeights->push_back(temp);
            _weightStats.average += temp;
            _weightStats.maximum = temp > _weightStats.maximum ? temp : _weightStats.maximum;
            _weightStats.minimum = temp < _weightStats.minimum ? temp : _weightStats.minimum;

            // Volume
            temp = option ? rand_range(_volume * 1e-3,_volume * 5e-3) : 0.0;
            _objVolumes->push_back(temp);
            _volumeStats.average += temp;
            _volumeStats.maximum = temp > _volumeStats.maximum ? temp : _volumeStats.maximum;
            _volumeStats.minimum = temp < _volumeStats.minimum ? temp : _volumeStats.minimum;

        }
        input.close();

        _weightStats.average = _weightStats.average/(double)_objWeights->size();
        _volumeStats.average = _volumeStats.average/(double)_objVolumes->size();



    } else {
        cout << "Could not open " << filename << "!" << endl;
        exit(1);
    }
}

std::shared_ptr<std::vector<double> > KnapsackLibrary::getVolumes()
{
    return _objVolumes;
}

std::shared_ptr<std::vector<double> > KnapsackLibrary::getWeights()
{
    return _objWeights;
}

std::shared_ptr<std::vector<double> > KnapsackLibrary::getValues()
{
    return _objValues;
}

double KnapsackLibrary::getValue(int i)
{
    int size = _objValues->size();
    if (i < 0 || i > size){
        cout << "KnapsackLibrary::getValue - Tried to access element outside vector!" << endl;
        exit(1);
    }
    return _objValues->at(i);
}

double KnapsackLibrary::getWeight(int i)
{
    int size = _objWeights->size();
    if (i < 0 || i > size){
        cout << "KnapsackLibrary::getWeight - Tried to access element outside vector!" << endl;
        exit(1);
    }
    return _objWeights->at(i);
}

double KnapsackLibrary::getVolume(int i)
{
    int size = _objVolumes->size();
    if (i < 0 || i > size){
        cout << "KnapsackLibrary::getVolume - Tried to access element outside vector!" << endl;
        exit(1);
    }
    return _objVolumes->at(i);
}

double KnapsackLibrary::getVolumeCapacity()
{
    return _volume;
}

statistics KnapsackLibrary::getWeightStats()
{
    return _weightStats;
}

statistics KnapsackLibrary::getVolumeStats()
{
    return _volumeStats;
}

void KnapsackLibrary::printAll()
{
    cout << "---------------------------\n KNAPSACK LIBRARY\n---------------------------" << endl;
    cout << "value\tweight\tvolume" << endl;
    for (int i = 0; i < _objValues->size(); i++)
    {
        cout << _objValues->at(i) << "\t" << _objWeights->at(i) << "\t" << _objVolumes->at(i) << endl;
    }
    cout << "---------------------------" << endl;
}

void KnapsackLibrary::printSummary()
{
    cout << "KNAPSACK LIBRARY SUMMARY" << endl;
    cout << "Number of packages = " << _objValues->size() << endl;
    cout << "Weights:" << endl;
    cout << "avg = " << _weightStats.average << endl;
    cout << "max = " << _weightStats.maximum << endl;
    cout << "min = " << _weightStats.minimum << endl;
    cout << "Volumes:" << endl;
    cout << "avg = " << _volumeStats.average << endl;
    cout << "max = " << _volumeStats.maximum << endl;
    cout << "min = " << _volumeStats.minimum << endl;
}

double KnapsackLibrary::getWeightCapacity()
{
    return _weight;
}
