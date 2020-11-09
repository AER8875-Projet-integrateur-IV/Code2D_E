/* 
/Fichier roeScheme.hpp
/Date: 09 novembre 2020 - 04h33
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

class RoeScheme
{
private:
    InputData *_inputData;
    MeshData *_meshData;
    Flux *_F;
    Solution *_W;
    Flux *_Fcentre;
    Flux *_Fdissip;

    vector<int> *_esuf;
    vector<double> *_face2Normales;
    void computeFluxDissip();
    void computeFluxCentres();

public:
    RoeScheme(MeshData *meshData, InputData *inputData, Flux *F, Solution *W);
    ~RoeScheme();
    void computeConvectivesFlux();
};