/* 
/Fichier schemes.cpp
/Date: 09 novembre 2020 - 04h14
/Projet: Projet Intégrateur 4
*/

#include "./schemes.hpp"

Schemes::Schemes(MeshData *meshData, InputData *inputData, Flux *F, Solution *W)
{
    _meshData = meshData;
    _inputData = inputData;
    _W = W;
    _F = F;
    _schema = _inputData->getScheme();
    _roeScheme = new RoeScheme(_meshData, _inputData, _F, _W);
    return;
};

Schemes::~Schemes()
{
    delete _roeScheme;
    return;
};
void Schemes::computeConvectivesFlux()
{
    if (_schema == "ROE")
    {
        _roeScheme->computeConvectivesFlux();
    }
    else
    {
        printf("Le schéma entré n'est pas reconnu (%s).\n", _schema.c_str());
        exit(1);
    };
    return;
};