/* 
/Fichier meshRead.cpp
/Date: 12 octobre 2020 - 19h36 
/Projet: Projet Intégrateur 4
*/

#include "./meshConnectivity.hpp"

MeshConnectivity::MeshConnectivity(shared_ptr<MeshData> meshData)
{
    _meshData = meshData;
    return;
};
MeshConnectivity::~MeshConnectivity()
{
    return;
};
