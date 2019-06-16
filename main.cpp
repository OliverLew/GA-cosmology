#include <iostream>
#include <fstream>
#include "lib/cos.hpp"
#include "lib/data.hpp"
#include <ga/ga.h>
#include <math.h>


std::ifstream datafile("data/C11.csv", std::ios::in);
Data Ia(datafile);

double objective(GAGenome &);

int main(int argc, char const *argv[])
{
    std::cout << "This program is try to solve the cosmology\n";

    // Declare variables for the GA parameters and set them to some default value.
    int popsize = 30;
    int ngen = 100;
    double pmut = 0.01;
    int pcross = 0.6;

    // 2DecPhenotype
    GABin2DecPhenotype map;
    map.add(16, 0, 1); // reduced hubble paramter
    map.add(16, 0, 1); // Omega_m
    map.add(16, 0, 1); // Omega_r

    return 0;
}

// This function tries to maximize the value of the function
double objective(GAGenome & c)
{
    GABin2DecGenome & genome = (GABin2DecGenome &) c;
    double h0 = genome.phenotype(0); 
    double Om = genome.phenotype(1);
    double Or = genome.phenotype(2);
    double Od = 1 - Om - Or;

    Cosmos cos(h0, Om, Or);
    std::vector<double> muth, muobs, sigma;
    muth = cos.dl(Ia.getz());
    muobs = Ia.getmu();
    sigma = Ia.getsigma();
    for (size_t i = 0; i < muth.size(); i++)
    {
        muth[i] = muth[i] / h0 / 100;
        muth[i] = 5 * log10(muth[i]) + 25;
    }
    double A = 0, B = 0, C = 0;
    for (size_t i = 0; i < muth.size(); i++)
    {
        A += (muth[i] - muobs[i]) * (muth[i] - muobs[i]) / sigma[i] / sigma[i];
        B += (muobs[i] - muth[i]) / sigma[i] * sigma[i];
        C += 1 / sigma[i] * sigma[i];
    }
    return - (A - B * B / C);
}

