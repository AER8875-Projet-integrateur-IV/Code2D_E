/* 
/Fichier meshData.hpp
/Date: 10 octobre 2020
/Projet: Projet Intégrateur 4
*/
#ifndef MESHDATA_HPP
#define MESHDATA_HPP

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class MeshData
{
private:
    // Paramètres du maillage
    int _NDIME;
    int _NELEM;
    int _NPOIN;
    int _NMARK;
    vector<string> _MARKER_TAG;
    vector<int> _MARKER_ELEMS;

    // Tableaux du maillage
    double *_nodes; // Coordonnées des noeuds
    vector<double> *_element2nodes;
    int *_element2nodesStart;
    int *_elementTypes;

    // Frontières
    vector<vector<double> *> *_element2nodesBoundary;
    vector<int *> *_element2nodesStartBoundary;
    vector<int *> *_boundaryTypes;

public:
    MeshData();
    ~MeshData();

    void setNDIME(int NDIME);
    void setNELEM(int NELEM);
    void setNPOIN(int NPOIN);
    void setNMARK(int NMARK);
    void setMARKER_TAG(string MARKER_TAG);
    void setMARKER_ELEMS(int MARKER_ELEMS);

    void setNodes(vector<string> nodes);
    void setElement2Nodes(vector<string> element2Nodes);
    void setElement2NodesFrontieres(vector<string> element2NodesFrontieres);
};
#endif