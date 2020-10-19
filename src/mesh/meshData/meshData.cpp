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

/* 
int MeshData::VTK2NFAEL(const int &vtkIndex)
{
    return 0;
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
*/
void MeshData::setEsuel()
{
    cout << "c°) Début de la génération de la connectivité element vs elements.\n";

    // Construction de _esuelStart
    _esuelStart.reserve(_NELEM + 1);
    _esuelStart.push_back(0);
    for (int iElem = 0; iElem < _NELEM; iElem++)
    {
        _esuelStart.push_back(VTKConnectivity::getNfael(_elementTypes[iElem]) + _esuelStart.back());
    }
    _NFACE = _esuelStart.back();
    _NBOUNDARY = 0;
    for (int iMark = 0; iMark < _NMARK; iMark++)
    {
        _NBOUNDARY += _MARKER_ELEMS[iMark];
    }
    _NFACE += _NBOUNDARY;
    _NFACE /= 2;

    // Construction de _esuel , _esuf et _fsuel

    _esuel.assign(_esuelStart.back(), -1);
    _fsuel.assign(_esuelStart.back(), -1);
    _esuf.assign(2 * _NFACE, -1);
    _psufStart.reserve(_NFACE + 1);
    _psufStart.push_back(0);
    _psuf.reserve(_NDIME * _NFACE);
    int iFace = 0;

    vector<int> lpoin(_NPOIN, 0);
    for (int iElem = 0; iElem < _NELEM; iElem++)
    {
        for (int iFael = 0; iFael < VTKConnectivity::getNfael(_elementTypes[iElem]); iFael++)
        {
            int nnofa = VTKConnectivity::getLnofa(_elementTypes[iElem], iFael);
            vector<int> lhelp = VTKConnectivity::getLpofa(_elementTypes[iElem], iFael);
            for (int i = 0; i < nnofa; i++)
            {
                lhelp[i] = _element2Nodes[_element2NodesStart[iElem] + lhelp[i]];
                lpoin[lhelp[i]] = 1;
            }
            int ipoin = lhelp[0];
            for (int istor = _esupStart[ipoin]; istor < _esupStart[ipoin + 1]; istor++)
            {
                int jElem = _esup[istor];
                if (jElem != iElem)
                {
                    for (int jFael = 0; jFael < VTKConnectivity::getNfael(_elementTypes[jElem]); jFael++)
                    {
                        int nnofj = VTKConnectivity::getLnofa(_elementTypes[jElem], jFael);
                        if (nnofj == nnofa)
                        {
                            int icoun = 0;
                            vector<int> lhelp_j = VTKConnectivity::getLpofa(_elementTypes[jElem], jFael);
                            for (int jnofa = 0; jnofa < nnofj; jnofa++)
                            {
                                lhelp_j[jnofa] = _element2Nodes[_element2NodesStart[jElem] + lhelp_j[jnofa]];
                                icoun += lpoin[lhelp_j[jnofa]];
                            }
                            if (icoun == nnofa)
                            {
                                _esuel[_esuelStart[iElem] + iFael] = jElem;

                                if (_esuel[_esuelStart[jElem] + jFael] == iElem)
                                {
                                    _fsuel[_esuelStart[iElem] + iFael] = _fsuel[_esuelStart[jElem] + jFael];
                                }
                                else
                                {
                                    // Adding a new face
                                    _fsuel[_esuelStart[iElem] + iFael] = iFace;
                                    _fsuel[_esuelStart[jElem] + jFael] = iFace;
                                    _esuf[iFace * 2 + 0] = iElem;
                                    _esuf[iFace * 2 + 1] = jElem;
                                    _psufStart.push_back(_psufStart.back() + nnofa);
                                    for (int inofa = 0; inofa < nnofa; inofa++)
                                    {
                                        _psuf.push_back(lhelp[inofa]);
                                    }
                                    iFace++;
                                }
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

void MeshData::setGhostCell()
{
    int iFace = _NFACE - _NBOUNDARY;
    int nelem = _NELEM;
    _face2bc.reserve(2 * _NBOUNDARY);
    for (int iElem = 0; iElem < _NELEM; iElem++)
    {
        for (int j = _esuelStart[iElem]; j < _esuelStart[iElem + 1]; j++)
        {
            if (_esuel[j] == -1)
            {
                int iFael = j - _esuelStart[iElem];
                _esuel[j] = nelem;
                _esuf[iFace * 2 + 0] = iElem;
                _esuf[iFace * 2 + 1] = nelem;
                _fsuel[_esuelStart[iElem] + iFael] = iFace;
                int nnofa = VTKConnectivity::getLnofa(_elementTypes[iElem], iFael);
                _psufStart.push_back(_psufStart.back() + nnofa);
                vector<int> lhelp = VTKConnectivity::getLpofa(_elementTypes[iElem], iFael);
                for (int i = 0; i < nnofa; i++)
                {
                    lhelp[i] = _element2Nodes[_element2NodesStart[iElem] + lhelp[i]];
                    _psuf.push_back(lhelp[i]);
                }
                for (int iMark = 0; iMark < _NMARK; iMark++)
                {
                    for (int jFael = 0; jFael < _MARKER_ELEMS[iMark]; jFael++)
                    {
                        vector<int> lhelp2;
                        getElement2NodesBoundary(iMark, jFael, lhelp2);
                        std::sort(lhelp.begin(), lhelp.end());
                        if (lhelp == lhelp2)
                        {
                            _face2bc.push_back(iFace);
                            _face2bc.push_back(iMark);
                        }
                    }
                }

                nelem++;
                iFace++;
            }
        }
    }

    return;
}

void MeshData::setConnectivity()
{
    setEsup();
    //setFaces();
    setEsuel();
    setGhostCell();
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

int MeshData::getNBOUNDARY() const
{
    return _NBOUNDARY;
}

int MeshData::getNMARK() const
{
    return _NMARK;
}

int MeshData::getNFACE() const
{
    return _NFACE;
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

void MeshData::getElement2NodesBoundary(int iMark, int iFael, vector<int> &lhelp)
{
    for (int i = _element2NodesStartBoundary[iMark][iFael]; i < _element2NodesStartBoundary[iMark][iFael + 1]; i++)
    {
        lhelp.push_back(_element2NodesBoundary[iMark][i]);
    }
    std::sort(lhelp.begin(), lhelp.end());

    return;
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

int MeshData::getNfael(const int &iElem)
{
    return VTKConnectivity::getNfael(_elementTypes[iElem]);
}

int MeshData::getLnofa(const int &iElem, const int &iFael)
{
    return VTKConnectivity::getLnofa(_elementTypes[iElem], iFael);
}

vector<int> MeshData::getLpofa(const int &iElem, const int &iFael)
{
    return VTKConnectivity::getLpofa(_elementTypes[iElem], iFael);
}

vector<int> MeshData::getEsuel() const
{
    return _esuel;
}

vector<int> MeshData::getEsuelStart() const
{
    return _esuelStart;
}

vector<int> MeshData::getFsuel() const
{
    return _fsuel;
}

vector<int> MeshData::getEsuf() const
{
    return _esuf;
}

vector<int> MeshData::getPsuf() const
{
    return _psuf;
}

vector<int> MeshData::getPsufStart() const
{
    return _psufStart;
}

vector<int> MeshData::getFace2bc() const
{
    return _face2bc;
}