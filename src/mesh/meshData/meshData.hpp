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
#include <algorithm>
#include <functional>

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
    int _NBOUNDARY;
    vector<string> _MARKER_TAG;
    vector<int> _MARKER_ELEMS;

    bool checkClockWise(vector<int> &nodes);

    // Tableaux du maillage
    vector<double> *_nodes; // Coordonnées des noeuds
    vector<int> *_element2Nodes_unsorted;
    vector<int> *_element2Nodes;
    vector<int> *_element2NodesStart;
    vector<int> *_elementTypes;
    bool _nodesReserve = false;
    bool _element2NodesReserve = false;

    // Frontières
    vector<vector<int>> *_element2NodesBoundary;
    vector<vector<int>> *_element2NodesStartBoundary;
    vector<vector<int>> *_elementTypesBoundary;
    bool _element2NodesBoundaryReserve = false;
    vector<bool> _element2NodesBoundaryReserves;

    // Connectivé

    /// Connectivité node2elements
    vector<int> *_esup;
    vector<int> *_esupStart;
    void setEsup();

    /// Connectivité element2elements
    vector<int> *_esuelStart;
    vector<int> *_esuel;

    /// Connectivité element2faces
    vector<int> *_fsuel;
    // _fsuelStart =  _esuelStart

    /// Connectivité face2elements
    vector<int> *_esuf;

    /// Connectivité face2nodes
    vector<int> *_psuf;
    vector<int> *_psufStart;
    void setEsuel();

    /// Cellules fantômes et conditions frontières
    vector<int> *_face2bc;
    vector<int> *_bc2el;
    vector<int> *_bc2elStart;
    void setGhostCell();
    void setBC();

    ////////////////////    METRIQUES        ///////////////////////////////////////////

    vector<double> *_element2Volumes;
    vector<double> *_element2Centres;
    vector<double> *_face2Aires;
    vector<double> *_face2Centres;
    vector<double> *_face2Normales;

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
    int getNBOUNDARY() const;
    int getNMARK() const;
    int getNFACE() const;
    vector<string> getMARKER_TAG() const;
    vector<int> getMARKER_ELEMS() const;

    /// Tableaux du maillage
    vector<double> *getNodes() const;
    vector<int> *getElement2Nodes() const;
    void getElement2Nodes(int &iElem, vector<int> &nodes) const;
    vector<int> *getElement2NodesStart() const;
    vector<int> *getElementTypes() const;
    vector<vector<int>> *getElement2NodesBoundary() const;
    vector<vector<int>> *getElement2NodesStartBoundary() const;
    vector<vector<int>> *getElementTypesBoundary() const;
    void getElement2NodesBoundary(int iMark, int iFael, vector<int> &lhelp);

    /// Connectivité
    vector<int> *getEsup() const;
    vector<int> *getEsupStart() const;
    int getVTKindex(const int &iElem);
    int getNfael(const int &iElem);
    int getLnofa(const int &iElem, const int &iFael);
    vector<int> getLpofa(const int &iElem, const int &iFael);
    void getFace2Nodes(int &iFace, vector<int> &nodes) const;
    vector<int> *getEsuel() const;
    vector<int> *getEsuelStart() const;
    vector<int> *getFsuel() const;
    vector<int> *getEsuf() const;
    vector<int> *getPsuf() const;
    vector<int> *getPsufStart() const;
    vector<int> *getFace2bc() const;
    vector<int> *getBc2el() const;
    vector<int> *getBc2elStart() const;

    /////////////////////////////////////////////////////////////////////////////////////

    /////////////////////    METRIQUES        ///////////////////////////////////////////

    // Setters

    void initializeMetric();
    void setElement2Volumes(const double &volume);
    void setElement2Centres(const vector<double> &centre);
    void setFace2Aires(const double &aire);
    void setFace2Centres(const vector<double> &centre);
    void setFace2Normales(const vector<double> &normale);

    // Getters

    vector<double> *getElement2Volumes() const;
    vector<double> *getElement2Centres() const;
    vector<double> *getFace2Aires() const;
    vector<double> *getFace2Centres() const;
    vector<double> *getFace2Normales() const;
};