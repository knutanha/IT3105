#ifndef POINTSET_H
#define POINTSET_H

#include <vector>
#include <string>
#include <memory>

class PointSet
{
public:
    PointSet(const std::string &filename);
    std::shared_ptr<std::vector<int> > getRandomOrder();
    std::pair<double,double> getWeights(int pointIndex);
    int getDimension();
    void print();
private:
    std::vector<std::pair<double,double> > _originalCoordinates;
    std::shared_ptr <std::vector<std::pair<double,double> > > _coordinates;
    std::string _name;
    int _dimension;
    std::pair<double, double> _yBounds;
    std::pair<double, double> _xBounds;
};

#endif // POINTSET_H
