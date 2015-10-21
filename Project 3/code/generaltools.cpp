#include "generaltools.h"
#include <math.h>
#include <random>
#include <iostream>

using namespace std;
// euclidean_distance
// Calculates distance between two points in an euclidean space

vector<double> subtract_vector(vector<double>& a, vector<double>& b){
    int size = a.size() < b.size() ? a.size() : b.size();
    vector<double> c;
    c.reserve(size);
    for (int i = 0; i < size; i++){
        double temp = a[i] - b[i];
        c.push_back(temp);
    }
    return c;
}

double euclidean_distance(vector<double>& a, vector<double>& b)
{
    int size = a.size()<=b.size()?a.size():b.size();
    double norm = 0.0;
    for (int i = 0; i < size; i++){
        norm += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sqrt(norm);
}



double rand_range(pair<double, double> limits){
    return (limits.first + ((double)rand()/RAND_MAX) * (limits.second - limits.first));
}
double rand_range(double lower, double upper){
    return (lower + ((double)rand()/RAND_MAX) * (upper - lower));
}
int rand_range(int lower, int upper){
    return (lower + (rand() % (upper - lower + 1)));
}

double sigmoid_map(double value)
{
    return 1.0/(1.0+exp(-value));
}


vector<int> unique_randoms(int lower, int upper, int n)
{
    // Using Fisher–Yates algorithm concept for selecting unique integers

    int absRange = upper - lower + 1;
    int temp, taken;

    if (n > absRange){
        cout << "unique_randoms: too many numbers requested!" << endl;
        exit(1);
    }

    vector<int> result;
    for (int i = 0; i < n; i++)
    {
        temp = rand() % absRange;
        taken = true;
        while (taken == true){
            taken = false;
            for (auto k : result) {
                if (k == temp)
                    taken = true;
            }
            if (taken==true){
                temp++;
            }
        }
        result.push_back(temp);
        absRange--;
    }

    for (int i = 0; i < n; i++)
    {
        result[i] = result[i] + lower;
    }
    return result;
}
