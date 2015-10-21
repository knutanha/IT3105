#ifndef KNAPSACKLIBRARY_H
#define KNAPSACKLIBRARY_H

#include <string>
#include <vector>
#include <memory>
#include "generaltools.h"

class KnapsackLibrary
{
public:

    KnapsackLibrary(double weight,
                    double volume,
                    const std::string filename,
                    bool option);

    // GET
    // vectors
    std::shared_ptr<std::vector<double> >
    getVolumes();

    std::shared_ptr<std::vector<double> >
    getWeights();

    std::shared_ptr<std::vector<double> >
    getValues();

    // vector elements
    double
    getValue(int i);

    double
    getWeight(int i);

    double
    getVolume(int i);

    // variables
    double
    getWeightCapacity();

    double
    getVolumeCapacity();

    // statistics
    statistics
    getWeightStats();

    statistics
    getVolumeStats();

    // IO
    void
    printAll();

    void
    printSummary();

private:

    statistics _weightStats;
    statistics _volumeStats;
    double _weight;
    double _volume;
    std::shared_ptr<std::vector<double> > _objVolumes;
    std::shared_ptr<std::vector<double> > _objWeights;
    std::shared_ptr<std::vector<double> > _objValues;
};

#endif // KNAPSACKLIBRARY_H
