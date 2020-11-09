/* 
/Fichier solutionStruct.hpp
/Date: 05 novembre 2020 - 02h50
/Projet: Projet Intégrateur 4
*/

#pragma once

#include <string>
#include <vector>

using namespace std;

struct Solution
{
    vector<double> rho;
    vector<double> U;
    vector<double> V;
    vector<double> E;
    vector<double> p;
    vector<double> H;
};

struct Flux
{
    vector<double> Vn;
};