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
#include <memory>

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
    vector<double> _nodes; // Coordonnées des noeuds
    vector<int> _element2Nodes;
    vector<int> _element2NodesStart;
    vector<int> _elementTypes;
    bool _nodesReserve = false;
    bool _element2NodesReserve = false;

    // Frontières
    vector<vector<int>> _element2NodesBoundary;
    vector<vector<int>> _element2NodesStartBoundary;
    vector<vector<int>> _elementTypesBoundary;
    bool _element2NodesBoundaryReserve = false;
    vector<bool> _element2NodesBoundaryReserves;

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
    void setElement2NodesFrontieres();
    void setElement2NodesFrontieres(vector<string> element2NodesFrontieres, int marker_index);

    vector<double> getNodes() const;
    vector<int> getElement2Nodes() const;
    vector<int> getElement2NodesStart() const;
    vector<vector<int>> getElement2NodesBoundary() const;
    vector<vector<int>> getElement2NodesStartBoundary() const;
};
#endif