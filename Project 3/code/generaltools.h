#ifndef GENERALTOOLS_H
#define GENERALTOOLS_H

#include <vector>

struct statistics{
    double average;
    double minimum;
    double maximum;
};

double rand_range(std::pair<double, double> limits);

double rand_range(double lower, double upper);

int rand_range(int lower, int upper);

std::vector<int> unique_randoms(int lower, int upper, int n);

double euclidean_distance(std::vector<double>& a, std::vector<double>& b);

double sigmoid_map(double value);

std::vector<double> subtract_vector(std::vector<double>& a, std::vector<double>& b);

#endif // GENERALTOOLS_H
