#include <iostream>
#include <vector>
#include <time.h>
#include "swarm.h"
#include "particleknapsack.h"

using namespace std;

int main()
{
    srand(time(NULL));
    cout << "Running PSO" << endl;
    int dim =1;
    pair<double,double> domain = pair<double,double>(-1, 1);
    //pair<double,double> vDomain = pair<double,double>(-4.25, 4.25);
    pair<double,double> vDomain = pair<double,double>(-0.05, 0.05);
    int npart = 100;

    double gaw = 1;
    double law = 1;
    double vw = 1;

    double maxit = 1000;
    double tol = 0.001;


    Swarm test(CircleProblem, dim, domain, vDomain, npart, 0, gaw, law,vw,true);

    //vector<shared_ptr<Particle> > lol = test.getNeighbours(0, 7);

    PSOResult result = test.runPSO(maxit, tol);

    result.printResult();

//    share

//    shared_ptr<KnapsackLibrary> lol = make_shared<KnapsackLibrary>(1000,1000,"pso-packages.txt",false);
//    lol->printSummary();

//    ParticleKnapsack rofl(0,dim,pair<double,double>(0,1),pair<double,double>(-4.25,4.25),gaw,law,vw,lol);

//    rofl.printSummary();

    //cout << lol.getVolumeCapacity();

    return 0;
}

