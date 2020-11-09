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

using namespace std;

class Schemes
{
private:
    InputData *_inputData;
    MeshData *_meshData;
    string _schema;
    RoeScheme *_roeScheme;
    Flux *_F;
    Solution *_W;

public:
    Schemes(MeshData *meshData, InputData *inputData, Flux *F, Solution *W);
    ~Schemes();
    void computeConvectivesFlux();
};