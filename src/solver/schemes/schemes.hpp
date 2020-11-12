/* 
/Fichier schemes.hpp
/Date: 09 novembre 2020 - 04h14
/Projet: Projet Intégrateur 4
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../solutionStruct.hpp"
#include "../mesh/meshData/meshData.hpp"
#include "../inputData.hpp"
#include "./roeScheme.hpp"
#include "./eulerExplicitScheme.hpp"

using namespace std;

class Schemes
{
private:
    InputData *_inputData;
    MeshData *_meshData;
    string _schemaSpace;
    string _schemaTime;

    RoeScheme *_roeScheme;
    EulerExplicitScheme *_eulerExplicitScheme;

    Flux *_F;
    Solution *_W;
    Residual *_R;
    Solution *_dW;
    vector<double> *_timeSteps;

public:
    Schemes(MeshData *meshData, InputData *inputData, Flux *F, Solution *W, Residual *R, Solution *dW, vector<double> *timeSteps);
    ~Schemes();
    void computeConvectivesFlux();
    void computeConservativesVariables();
};