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

/* void MeshConnectivity::generateEsup()
{
    int NELEM = _meshData->getNELEM();
    _meshData->setEsupInitialize(); // Initialisation de MeshData._esupStart
    for (int i = 0; i < NELEM; i++)
    {
        for (int j = _meshData->getElement2NodesStart()[i]; j < _meshData->getElement2NodesStart()[i + 1]; j++)
        {
            int ipoin = _meshData->getElement2Nodes()[j];
        }
    }

    return;
} */
