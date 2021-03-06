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
    vector<double> rhoU;
    vector<double> rhoV;
    vector<double> rhoE;
    vector<double> p;
    vector<double> H;
};

struct Flux
{
    vector<double> rhoV;
    vector<double> rhouV;
    vector<double> rhovV;
    vector<double> rhoHV;
};

struct Residual
{
    vector<double> rhoVds;
    vector<double> rhouVds;
    vector<double> rhovVds;
    vector<double> rhoHVds;
};