/* 
/Fichier meshData.cpp
/Date: 10 octobre 2020
/Projet: Projet Intégrateur 4
*/
#include "./meshData.hpp"

MeshData::MeshData()
{
    return;
}

MeshData::~MeshData()
{
    return;
}

// Setters

void MeshData::setNDIME(int NDIME)
{
    _NDIME = NDIME;
}

void MeshData::setNELEM(int NELEM)
{
    _NELEM = NELEM;
}

void MeshData::setNPOIN(int NPOIN)
{
    _NPOIN = NPOIN;
}

void MeshData::setNMARK(int NMARK)
{
    _NMARK = NMARK;
}

void MeshData::setMARKER_TAG(string MARKER_TAG)
{
    _MARKER_TAG.push_back(MARKER_TAG);
}

void MeshData::setMARKER_ELEMS(int MARKER_ELEMS)
{
    _MARKER_ELEMS.push_back(MARKER_ELEMS);
}

void MeshData::setNodes(vector<string> nodes)
{
    if (!_nodesReserve)
    {
        _nodes.reserve(_NDIME * _NPOIN);
        _nodesReserve = true;
    }
    for (int i = 0; i < _NDIME; i++)
    {
        _nodes.push_back(stod(nodes[i]));
    }
    return;
}

void MeshData::setElement2Nodes(vector<string> element2Nodes)
{
    if (!_element2NodesReserve)
    {
        _element2Nodes.reserve(_NELEM * (element2Nodes.size() - 2));
        _element2NodesStart.reserve(_NELEM + 1);
        _element2NodesStart.push_back(0);
        _elementTypes.reserve(_NELEM);
        _element2NodesReserve = true;
    }
    _elementTypes.push_back(stoi(element2Nodes[0]));
    for (size_t i = 1; i < element2Nodes.size() - 1; i++)
    {
        _element2Nodes.push_back(stoi(element2Nodes[i]));
    }
    _element2NodesStart.push_back(element2Nodes.size() - 2);
    return;
}

void MeshData::setElement2NodesFrontieres()
{
    if (!_element2NodesBoundaryReserve)
    {
        _element2NodesBoundary.reserve(_NMARK);
        _element2NodesStartBoundary.reserve(_NMARK);
        _elementTypesBoundary.reserve(_NMARK);
        _element2NodesBoundaryReserve = true;
        _element2NodesBoundaryReserves.assign(_NMARK, false);
    }
    return;
}

void MeshData::setElement2NodesFrontieres(vector<string> element2NodesFrontieres, int marker_index)
{
    if (!_element2NodesBoundaryReserves[marker_index - 1])
    {
        vector<int> vecteur_temp1;
        vecteur_temp1.reserve(_MARKER_ELEMS[marker_index - 1] * (element2NodesFrontieres.size() - 1));
        _element2NodesBoundary.push_back(vecteur_temp1);
        vector<int> vecteur_temp2;
        vecteur_temp2.reserve(_MARKER_ELEMS[marker_index - 1] + 1);
        _element2NodesStartBoundary.push_back(vecteur_temp2);
        _element2NodesStartBoundary[marker_index - 1].push_back(0);
        vector<int> vecteur_temp3;
        vecteur_temp3.reserve(_MARKER_ELEMS[marker_index - 1]);
        _elementTypesBoundary.push_back(vecteur_temp3);
        _element2NodesBoundaryReserves[marker_index - 1] = true;
    }
    _elementTypesBoundary[marker_index - 1].push_back(stoi(element2NodesFrontieres[0]));
    for (size_t i = 1; i < element2NodesFrontieres.size(); i++)
    {
        _element2NodesBoundary[marker_index - 1].push_back(stoi(element2NodesFrontieres[i]));
    }
    _element2NodesStartBoundary[marker_index - 1].push_back(element2NodesFrontieres.size() - 2);
    return;
}

//Getters
