/* 
/Fichier schemes.cpp
/Date: 09 novembre 2020 - 04h14
/Projet: Projet Intégrateur 4
*/

#include "./schemes.hpp"

Schemes::Schemes(MeshData *meshData, InputData *inputData, Flux *F, Solution *W, Residual *R, Solution *dW, vector<double> *timeSteps)
{
    _meshData = meshData;
    _inputData = inputData;
    _W = W;
    _F = F;
    _dW = dW;
    _R = R;
    _timeSteps = timeSteps;
    _schemaSpace = _inputData->getSchemeSpace();
    _schemaSpace = _inputData->getSchemeTime();
    _roeScheme = new RoeScheme(_meshData, _inputData, _F, _W);
    _eulerExplicitScheme = new EulerExplicitScheme(_meshData, _R, _dW, _timeSteps);
    return;
};

Schemes::~Schemes()
{
    delete _roeScheme;
    delete _eulerExplicitScheme;
    return;
};

void Schemes::computeConvectivesFlux()
{
    if (_schemaSpace == "ROE")
    {
        _roeScheme->computeConvectivesFlux();
    }
    else
    {
        printf("Le schéma entré n'est pas reconnu (%s).\n", _schemaSpace.c_str());
        exit(1);
    };
    return;
};

void Schemes::computeConservativesVariables()
{
    if (_schemaTime == "EULER EXPLICITE")
    {
        _eulerExplicitScheme->computeConservativesVariables();
    }
    else
    {
        printf("Le schéma entré n'est pas reconnu (%s).\n", _schemaTime.c_str());
        exit(1);
    };
    return;
};