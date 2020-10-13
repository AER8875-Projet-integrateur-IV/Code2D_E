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
    _element2NodesStart.push_back(_element2NodesStart.back() + element2Nodes.size() - 2);
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
    _element2NodesStartBoundary[marker_index - 1].push_back(_element2NodesStartBoundary[marker_index - 1].back() + element2NodesFrontieres.size() - 1);
    return;
}

/// Connectivité

void MeshData::setEsup()
{
    cout << "a°) Début de génération connectivité noeud vs elements.\n";
    // Initialisation de _esupStart

    _esupStart.assign(_NPOIN + 1, 0);

    // Première passe et stockage

    for (size_t i = 0; i < _element2Nodes.size(); i++)
    {
        _esupStart[_element2Nodes[i] + 1]++;
    }
    for (int i = 1; i < _NPOIN + 1; i++)
    {
        _esupStart[i] += _esupStart[i - 1];
    }

    // Deuxième passe et stockage

    _esup.assign(_esupStart.back(), 0);
    for (int iElem = 0; iElem < _NELEM; iElem++)
    {
        for (int iNode = _element2NodesStart[iElem]; iNode < _element2NodesStart[iElem + 1]; iNode++)
        {
            int nodeI = _element2Nodes[iNode];
            int istor = _esupStart[nodeI];
            _esupStart[nodeI]++;
            _esup[istor] = iElem;
        }
    }
    for (int iNode = _NPOIN; iNode > 0; iNode--)
    {
        _esupStart[iNode] = _esupStart[iNode - 1];
    }
    _esupStart[0] = 0;

    cout << "\t Fin génération connectivité noeud vs elements.\n";
    cout << "-------------------------------------------------------\n";

    return;
}

void MeshData::getVTKConnectivity(int vtkIndex, vector<vector<int>> &ilpofa, int iElem)
{
    switch (vtkIndex)
    {
    case 1:
        // VERTEX
        break;
    case 2:
        // POLY_VERTEX
        break;
    case 3:
        // LINE
        break;
    case 4:
        // POLY_LINE
        break;
    case 5:
        // TRIANGLE
        ilpofa.reserve(3);
        for (size_t i = 0; i < 3; i++)
        {
            vector<int> nodes_temp;
            nodes_temp.reserve(2);
            nodes_temp.push_back(_element2Nodes[_element2NodesStart[iElem] + i]);
            if (i == 2)
            {
                nodes_temp.push_back(_element2Nodes[_element2NodesStart[iElem]]);
            }
            else
            {
                nodes_temp.push_back(_element2Nodes[_element2NodesStart[iElem] + i + 1]);
            }
            ilpofa.push_back(nodes_temp);
        }
        break;
    case 6:
        // TRIANGLE_STRIP
        break;
    case 7:
        // POLYGON
        break;
    case 8:
        // PIXEL
        ilpofa.reserve(4);
        for (size_t i = 0; i < 4; i++)
        {
            vector<int> nodes_temp;
            nodes_temp.reserve(2);
            nodes_temp.push_back(_element2Nodes[_element2NodesStart[iElem] + i]);
            if (i == 3)
            {
                nodes_temp.push_back(_element2Nodes[_element2NodesStart[iElem]]);
            }
            else
            {
                nodes_temp.push_back(_element2Nodes[_element2NodesStart[iElem] + i + 1]);
            }
            ilpofa.push_back(nodes_temp);
        }
        break;
    case 9:
        // QUAD
        ilpofa.reserve(4);
        for (size_t i = 0; i < 4; i++)
        {
            vector<int> nodes_temp;
            nodes_temp.reserve(2);
            nodes_temp.push_back(_element2Nodes[_element2NodesStart[iElem] + i]);
            if (i == 3)
            {
                nodes_temp.push_back(_element2Nodes[_element2NodesStart[iElem]]);
            }
            else
            {
                nodes_temp.push_back(_element2Nodes[_element2NodesStart[iElem] + i + 1]);
            }
            ilpofa.push_back(nodes_temp);
        }
        break;
    case 10:
        // TETRA
        break;
    case 11:
        // VOXEL
        break;
    case 12:
        // HEXAHEDRON
        break;
    case 13:
        // WEDGE
        break;
    case 14:
        // PYRAMID
        break;
    default:
        break;
    }
    return;
}

void MeshData::setNNOFA(int iElem, vector<vector<int>> &ilpofa)
{
    int vtkIndex = _elementTypes[iElem];
    getVTKConnectivity(vtkIndex, ilpofa, iElem);
    return;
}

void MeshData::setFaces()
{
    cout << "b°) Début de la génération de la connectivité des faces.\n";
    _NFAEL.reserve(_NELEM);
    _NNOFA.reserve(_NELEM);
    _lpofa.reserve(_NELEM);
    for (int iElem = 0; iElem < _NELEM; iElem++)
    {
        vector<vector<int>> ilpofa;
        setNNOFA(iElem, ilpofa);
        _lpofa.push_back(ilpofa);
        _NFAEL.push_back(static_cast<int>(ilpofa.size()));
        vector<int> iNNOFA;
        iNNOFA.reserve(ilpofa.size());
        for (size_t iFace = 0; iFace < ilpofa.size(); iFace++)
        {
            iNNOFA.push_back(static_cast<int>(ilpofa[iFace].size()));
        }
        _NNOFA.push_back(iNNOFA);
    }
    cout << "\tFin de la génération de la connectivité des faces.\n";
    cout << "-----------------------------------------------------------\n";
    return;
}

void MeshData::setEsuel()
{
    cout << "c°) Début de la génération de la connectivité element vs elements.\n";

    // Construction de _esuelStart

    _esuelStart.reserve(_NELEM + 1);
    _esuelStart.push_back(0);
    for (size_t i = 0; i < _NFAEL.size(); i++)
    {
        _esuelStart.push_back(_NFAEL[i] + _esuelStart.back());
    }

    // Construction de _esuel

    _esuel.assign(_esuelStart.back(), -1);
    vector<int> lpoin(_NPOIN, 0);
    for (int iElem = 0; iElem < _NELEM; iElem++)
    {
        for (int iFael = 0; iFael < _NFAEL[iElem]; iFael++)
        {
            int nnofa = _NNOFA[iElem][iFael];
            vector<int> lhelp = _lpofa[iElem][iFael];
            for (size_t i = 0; i < lhelp.size(); i++)
            {
                lpoin[lhelp[i]] = 1;
            }
            int ipoin = lhelp[0];
            for (int istor = _esupStart[ipoin]; istor < _esupStart[ipoin + 1]; istor++)
            {
                int jElem = _esup[istor];
                if (jElem != iElem)
                {
                    for (int jFael = 0; jFael < _NFAEL[jElem]; jFael++)
                    {
                        int nnofj = _NNOFA[jElem][jFael];
                        if (nnofj != nnofa)
                        {
                            int icoun = 0;
                            for (int jnofa = 0; jnofa < nnofa; jnofa++)
                            {
                                int jpoin = _lpofa[jElem][jFael][jnofa];
                                icoun += lpoin[jpoin];
                            }
                            if (icoun == nnofa)
                            {
                                _esuel[_esuelStart[iElem] + iFael] = jElem;
                            }
                        }
                    }
                }
            }
            // Reset lpoin
            for (size_t i = 0; i < lhelp.size(); i++)
            {
                lpoin[lhelp[i]] = 0;
            }
        }
    }
    cout << "\tFin de la génération de la connectivité element vs elements.\n";
    cout << "-------------------------------------------------------------------\n";

    return;
}

void MeshData::setConnectivity()
{
    setEsup();
    setFaces();
    setEsuel();
    return;
}

// Getters

/// Paramètres du maillage

int MeshData::getNDIME() const
{
    return _NDIME;
}

int MeshData::getNELEM() const
{
    return _NELEM;
}

int MeshData::getNPOIN() const
{
    return _NPOIN;
}

int MeshData::getNMARK() const
{
    return _NMARK;
}

vector<string> MeshData::getMARKER_TAG() const
{
    return _MARKER_TAG;
}

vector<int> MeshData::getMARKER_ELEMS() const
{
    return _MARKER_ELEMS;
}

/// Tableaux du maillage

vector<double> MeshData::getNodes() const
{
    return _nodes;
}

vector<int> MeshData::getElement2Nodes() const
{
    return _element2Nodes;
}

vector<int> MeshData::getElement2NodesStart() const
{
    return _element2NodesStart;
}

vector<int> MeshData::getElementTypes() const
{
    return _elementTypes;
}

vector<vector<int>> MeshData::getElement2NodesBoundary() const
{
    return _element2NodesBoundary;
}

vector<vector<int>> MeshData::getElement2NodesStartBoundary() const
{
    return _element2NodesStartBoundary;
}

vector<vector<int>> MeshData::getElementTypesBoundary() const
{
    return _elementTypesBoundary;
}

/// Connectivité

vector<int> MeshData::getEsup() const
{
    return _esup;
}

vector<int> MeshData::getEsupStart() const
{
    return _esupStart;
}

vector<int> MeshData::getNFAEL() const
{
    return _NFAEL;
}

vector<vector<int>> MeshData::getNNOFA() const
{
    return _NNOFA;
}

vector<vector<vector<int>>> MeshData::getLpofa() const
{
    return _lpofa;
}

vector<int> MeshData::getEsuel() const
{
    return _esuel;
}

vector<int> MeshData::getEsuelStart() const
{
    return _esuelStart;
}