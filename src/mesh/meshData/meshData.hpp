/* 
/Fichier meshData.hpp
/Date: 10 octobre 2020
/Projet: Projet Intégrateur 4
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "./vtkConnectivity.hpp"

using namespace std;

class MeshData
{
private:
    // Paramètres du maillage
    int _NDIME;
    int _NELEM;
    int _NPOIN;
    int _NMARK;
    int _NFACE;
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

    // Connectivé

    /// Connectivité node2elements
    vector<int> _esup;
    vector<int> _esupStart;
    void setEsup();

    /// Connectivité des faces
    /*   
    vector<int> _NFAEL;
    vector<vector<int>> _NNOFA;
    vector<vector<vector<int>>> _lpofa;
    void getVTKConnectivity(int vtkIndex, vector<vector<int>> &ilpofa, int iElem);
    void setNNOFA(int iElem, vector<vector<int>> &ilpofa);
    void setFaces();
    int VTK2NFAEL(const int &vtkIndex); 
    */

    /// Connectivité element2elements
    vector<int> _esuelStart;
    vector<int> _esuel;
    vector<int> _fsuel;
    vector<int> _esuf;
    void setEsuel();

public:
    MeshData();
    ~MeshData();

    // Setters

    /// Paramètres du maillage
    void setNDIME(int NDIME);
    void setNELEM(int NELEM);
    void setNPOIN(int NPOIN);
    void setNMARK(int NMARK);
    void setMARKER_TAG(string MARKER_TAG);
    void setMARKER_ELEMS(int MARKER_ELEMS);

    /// Tableaux du maillage
    void setNodes(vector<string> nodes);
    void setElement2Nodes(vector<string> element2Nodes);
    void setElement2NodesFrontieres();
    void setElement2NodesFrontieres(vector<string> element2NodesFrontieres, int marker_index);

    /// Connectivité
    void setConnectivity();

    // Getters

    /// Paramètres du maillage
    int getNDIME() const;
    int getNELEM() const;
    int getNPOIN() const;
    int getNMARK() const;
    int getNFACE() const;
    vector<string> getMARKER_TAG() const;
    vector<int> getMARKER_ELEMS() const;

    /// Tableaux du maillage
    vector<double> getNodes() const;
    vector<int> getElement2Nodes() const;
    vector<int> getElement2NodesStart() const;
    vector<int> getElementTypes() const;
    vector<vector<int>> getElement2NodesBoundary() const;
    vector<vector<int>> getElement2NodesStartBoundary() const;
    vector<vector<int>> getElementTypesBoundary() const;

    /// Connectivité
    vector<int> getEsup() const;
    vector<int> getEsupStart() const;
    int getNfael(const int &iElem);
    int getLnofa(const int &iElem, const int &iFael);
    vector<int> getLpofa(const int &iElem, const int &iFael);
    vector<int> getEsuel() const;
    vector<int> getEsuelStart() const;
    vector<int> getFsuel() const;
    vector<int> getEsuf() const;
};