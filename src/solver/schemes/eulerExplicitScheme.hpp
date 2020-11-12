/* 
/Fichier roeScheme.hpp
/Date: 11 novembre 2020 - 21h26
/Projet: Projet Intégrateur 4
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include "../solutionStruct.hpp"
#include "../mesh/meshData/meshData.hpp"
#include "../inputData.hpp"

using namespace std;

class EulerExplicitScheme
{
private:
    MeshData *_meshData;
    Solution *_dW;
    Residual *_R;
    vector<double> *_timeSteps;

    vector<double> *_element2Volumes;

public:
    EulerExplicitScheme(MeshData *meshData, Residual *R, Solution *dW, vector<double> *timeSteps);
    ~EulerExplicitScheme();
    void computeConservativesVariables();
};