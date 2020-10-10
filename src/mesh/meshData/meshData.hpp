#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class MeshData
{
private:
    // Paramètres du maillage
    int NDIME;
    int NELEM;
    int NPOIN;
    int NMARK;
    vector<string> MARKER_TAG;
    vector<int> MARKER_ELEMS;

    // Tableaux du maillage
    double *nodes; // Coordonnées des noeuds
    vector<double> *element2nodes;
    int *element2nodesStart;
    int *elementTypes;

    // Frontières
    vector<vector<double> *> *element2nodesBoundary;
    vector<int *> *element2nodesStartBoundary;
    vector<int *> *boundaryTypes;

public:
    MeshData();
    ~MeshData();
};