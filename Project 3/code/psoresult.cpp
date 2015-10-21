#include "psoresult.h"
#include <vector>
#include <iostream>

using namespace std;

PSOResult::PSOResult(int it,
                     int maxit,
                     double globfit,
                     double globfitreq,
                     double score,
                     vector<double> position)
{
    _iterations = it;
    _maxIt = maxit;
    _globalFitness = globfit;
    _globalFitnessRequirement = globfitreq;
    _score = score;
    _position = position;
}

void PSOResult::printResult()
{
    cout << "\n\nPSO SEARCH REPORT\n" << endl;
    cout << "Settings:" << endl;
    cout << "maxit = " << _maxIt << endl;
    cout << "globFitReq = " << _globalFitnessRequirement << endl;
    cout << "\nResults: \n";
    cout << "it = " << _iterations << endl;
    cout << "globFit = " << _globalFitness << endl;
    cout << "best score = " << _score << endl;
    cout << "best position = [";
    for (auto i : _position){
        cout << i << " ";
    }
    cout << "]" << endl;
}
