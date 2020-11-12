/* 
/Fichier roeScheme.cpp
/Date: 09 novembre 2020 - 04h33
/Projet: Projet Intégrateur 4
*/

#include "./eulerExplicitScheme.hpp"

EulerExplicitScheme::EulerExplicitScheme(MeshData *meshData, Residual *R, Solution *dW, vector<double> *timeSteps)
{
    _meshData = meshData;
    _R = R;
    _dW = dW;
    _timeSteps = timeSteps;

    _element2Volumes = _meshData->getElement2Volumes();

    return;
}

EulerExplicitScheme::~EulerExplicitScheme()
{

    return;
}

void EulerExplicitScheme::computeConservativesVariables()
{
    for (int iElem = 0; iElem < _meshData->getNELEM(); iElem++)
    {
        _dW->rho[iElem] = -_timeSteps->at(iElem) * _R->rhoVds[iElem] / _element2Volumes->at(iElem);
        _dW->rhoU[iElem] = -_timeSteps->at(iElem) * _R->rhouVds[iElem] / _element2Volumes->at(iElem);
        _dW->rhoV[iElem] = -_timeSteps->at(iElem) * _R->rhovVds[iElem] / _element2Volumes->at(iElem);
        _dW->rhoE[iElem] = -_timeSteps->at(iElem) * _R->rhoHVds[iElem] / _element2Volumes->at(iElem);
    }

    return;
}
