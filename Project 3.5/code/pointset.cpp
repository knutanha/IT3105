#include "pointset.h"

#include <string>
#include <regex>
#include <iostream>
#include <sstream>
#include <limits>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;

//const regex TSP_COMMENT("(COMMENT)(.*)");
const regex TSP_NAME("(NAME)(.*)");
const regex TSP_DIMENSION("(DIMENSION)(.*)");
const regex TSP_COORDINATE("(\\d+)( *)(.*)");
const regex TSP_COORDINATE_PARSE("\\d+\\.?\\d*");

std::vector<string> &stringSplit(const string &s, char splitCharacter, vector<string> &elements) {
    stringstream ss(s);
    string section;
    while (std::getline(ss, section, splitCharacter)) {
        elements.push_back(section);
    }
    return elements;
}

pair<double,double> coordinateParser(string line)
{
    pair<double,double> coordinates;
    vector <string> numbers;
    numbers = stringSplit(line, ' ', numbers);
    coordinates.first = atof(numbers[1].c_str());
    coordinates.second = atof(numbers[2].c_str());
    return coordinates;
}

PointSet::PointSet(const string &filename)
{
    // tsp file parser
    fstream fileStream(filename);
    string line;

    if (fileStream.is_open() == false) {
        cout<< "Not a valid file!" << endl;
        exit(1);
    }
    double positiveInfinity = numeric_limits<double>::max();
    double negativeInfinity = numeric_limits<double>::min();

    _coordinates = make_shared< vector<pair<double,double> > >();

    _yBounds = pair<double,double>(positiveInfinity, negativeInfinity);
    _xBounds = _yBounds;
    pair<double, double> tempCoordinates;

    while (getline(fileStream,line))
    {
        if (regex_match (line, TSP_COORDINATE)) {
            tempCoordinates = coordinateParser(line);
            _coordinates->push_back (tempCoordinates);

            // Cheking if boundaries needs to be expanded
            _xBounds.first   = (tempCoordinates.first  < _xBounds.first ) ? tempCoordinates.first  : _xBounds.first;
            _xBounds.second  = (tempCoordinates.first  > _xBounds.second) ? tempCoordinates.first  : _xBounds.second;
            _yBounds.first   = (tempCoordinates.second < _yBounds.first ) ? tempCoordinates.second : _yBounds.first;
            _yBounds.second  = (tempCoordinates.second > _yBounds.second) ? tempCoordinates.second : _yBounds.second;
        }
    }

    //Assigning name and dimension
    _name = filename;
    _dimension = static_cast<int>(_coordinates->size());

    // Normalizing coordinates (less nightmares)

    double xDifference = _xBounds.second - _xBounds.first;
    double yDifference = _yBounds.second - _yBounds.first;
    for (int i = 0; i < _dimension; i++)
    {
        _originalCoordinates.push_back(_coordinates->at(i) ) ;
        _coordinates->at(i).first = (_coordinates->at(i).first - _xBounds.first) / xDifference;
        _coordinates->at(i).second = (_coordinates->at(i).second - _yBounds.first) / yDifference;
    }

    // Create MATLAB formated text file
    fstream stream;
    string matfile = _name + "m";
    stream.open(matfile, ios::out | ios::trunc);
    if (stream.is_open()){
        stream << "x\ty" << endl;
        for (auto i : *_coordinates){
            stream << i.first << "\t" << i.second << endl;
        }
        stream.close();
    }
    else
        cout << "PointSet: Could not create matlab file of city!" << endl;
}


shared_ptr<vector<int> > PointSet::getRandomOrder()
{
    shared_ptr<vector<int> > output = make_shared<vector<int> >();

    for (int i = 0; i < _dimension; i++)
        output->push_back(i);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(output->begin(),output->end(), default_random_engine(seed));

//    cout << "Random Order:" << endl;
//    for (auto i : *output)
//        cout << i << " ";
//    cout << endl;
    return output;
}

std::pair<double, double> PointSet::getWeights(int pointIndex)
{
    return _coordinates->at(pointIndex);
}

int PointSet::getDimension()
{
    return _dimension;
}

void PointSet::print()
{
    cout << "Name: " << _name << endl;
    cout << "Dimension: " << _dimension << endl;
    cout << "xRange = (" << _xBounds.first << ", " << _xBounds.second << ")" << endl;
    cout << "yRange = (" << _yBounds.first << ", " << _yBounds.second << ")" << endl;

    for (int i = 0; i < _coordinates->size(); i++){
        cout << "(" << i << "): " << _coordinates->at(i).first <<  ", " << _coordinates->at(i).second << endl;
    }
}
