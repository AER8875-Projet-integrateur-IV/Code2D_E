/* 
/Fichier meshMetric.cpp
/Date: 21 octobre 2020 - 02h13
/Projet: Projet Intégrateur 4
*/

#include "./meshMetric.hpp"

MeshMetric::MeshMetric(MeshData *meshData)
{
    _meshData = meshData;
    _meshData->initializeMetric();
    _NDIME = _meshData->getNDIME();
    _nodes = _meshData->getNodes();
    return;
}

MeshMetric::~MeshMetric()
{
    return;
}

void MeshMetric::calculAireTriangle(vector<double> &pt1, vector<double> &pt2, vector<double> &pt3, double &aire, vector<double> &centre)
{
    aire = 0.5 * ((pt1[0] - pt2[0]) * (pt1[1] + pt2[1]) + (pt2[0] - pt3[0]) * (pt2[1] + pt3[1]) + (pt3[0] - pt1[0]) * (pt3[1] + pt1[1]));
    centre.push_back((pt1[0] + pt2[0] + pt3[0]) / 3);
    centre.push_back((pt1[1] + pt2[1] + pt3[1]) / 3);
    return;
}

void MeshMetric::calculTriangle(vector<int> &nodes, double &aire, vector<double> &centre)
{
    vector<double> pt1 = {_nodes->at(nodes[0] * 2), _nodes->at(nodes[0] * 2 + 1)};
    vector<double> pt2 = {_nodes->at(nodes[1] * 2), _nodes->at(nodes[1] * 2 + 1)};
    vector<double> pt3 = {_nodes->at(nodes[2] * 2), _nodes->at(nodes[2] * 2 + 1)};
    calculAireTriangle(pt1, pt2, pt3, aire, centre);
    return;
}

void MeshMetric::calculQuad(vector<int> &nodes, double &aire, vector<double> &centre)
{
    vector<double> pt1 = {_nodes->at(nodes[0] * 2), _nodes->at(nodes[0] * 2 + 1)};
    vector<double> pt2 = {_nodes->at(nodes[1] * 2), _nodes->at(nodes[1] * 2 + 1)};
    vector<double> pt3 = {_nodes->at(nodes[2] * 2), _nodes->at(nodes[2] * 2 + 1)};
    vector<double> pt4 = {_nodes->at(nodes[3] * 2), _nodes->at(nodes[3] * 2 + 1)};
    double aire1, aire2;
    vector<double> centre1, centre2;
    calculAireTriangle(pt1, pt2, pt3, aire1, centre1);
    calculAireTriangle(pt3, pt4, pt1, aire2, centre2);
    aire = aire1 + aire2;
    centre.push_back((aire1 * centre1[0] + aire2 * centre2[0]) / aire);
    centre.push_back((aire1 * centre1[1] + aire2 * centre2[1]) / aire);
    return;
}

void MeshMetric::setElements()
{
    for (int iElem = 0; iElem < _meshData->getNELEM(); iElem++)
    {
        double aire;
        vector<double> centre;
        vector<int> nodes;
        _meshData->getElement2Nodes(iElem, nodes);
        int vtkIndex = _meshData->getVTKindex(iElem);
        if (vtkIndex == 5) // Triangle
        {
            calculTriangle(nodes, aire, centre);
        }
        else if (vtkIndex == 9) // Quad
        {
            calculQuad(nodes, aire, centre);
        }
        _meshData->setElement2Volumes(aire);
        _meshData->setElement2Centres(centre);
    }
};

void MeshMetric::setFaces()
{
    for (int iFace = 0; iFace < _meshData->getNFACE(); iFace++)
    {
        vector<int> nodes;
        _meshData->getFace2Nodes(iFace, nodes);
        // En 2D on a:
        vector<double> pt1 = {_nodes->at(nodes[0] * 2), _nodes->at(nodes[0] * 2 + 1)};
        vector<double> pt2 = {_nodes->at(nodes[1] * 2), _nodes->at(nodes[1] * 2 + 1)};
        double aire = sqrt(pow(pt2[1] - pt1[1], 2) + pow(pt2[0] - pt1[0], 2));
        vector<double> normale = {(pt2[1] - pt1[1]) / aire, (pt2[0] - pt1[0]) / aire};
        vector<double> centre = {(pt2[0] + pt1[0]) / 2., (pt2[1] + pt1[1]) / 2.};
        _meshData->setFace2Aires(aire);
        _meshData->setFace2Centres(centre);
        _meshData->setFace2Normales(normale);
    }
};

void MeshMetric::setMetric()
{
    setElements();
    setFaces();
    return;
}
