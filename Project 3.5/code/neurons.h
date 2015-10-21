#ifndef NEURONS_H
#define NEURONS_H

#include <vector>
#include <memory>

class Neurons
{
public:
    Neurons();

    Neurons(int numberOfNeurons);

    std::pair<double, double>
    getWeight(int neuron);

    std::shared_ptr<std::vector<std::pair<double,double> > >
    getWeights();

    int
    getDimension();

    void
    setWeight(int neuron, double x, double y);

    void
    setWeight(int neuron, std::pair<double,double> weight);

    void
    print();

private:
    std::shared_ptr<std::vector<std::pair<double,double> > > _neurons;
    int _numberOfNeurons;
};

#endif // NEURONS_H
