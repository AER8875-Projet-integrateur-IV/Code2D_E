/* 
/Fichier meshData.cpp
/Date: 10 octobre 2020
/Projet: Projet Intégrateur 4
*/

#include "./meshData.hpp"

MeshData::MeshData()
{
    // Maillage lu
    _nodes = new vector<double>();
    _element2Nodes_unsorted = new vector<int>();
    _element2Nodes = new vector<int>();
    _element2NodesStart = new vector<int>();
    _elementTypes = new vector<int>();
    // Conditions frontières
    _element2NodesBoundary = new vector<vector<int>>();
    _element2NodesStartBoundary = new vector<vector<int>>();
    _elementTypesBoundary = new vector<vector<int>>();
    // Connectivités
    _esup = new vector<int>();
    _esupStart = new vector<int>();
    _esuel = new vector<int>();
    _esuelStart = new vector<int>();
    _fsuel = new vector<int>();
    _esuf = new vector<int>();
    _psuf = new vector<int>();
    _psufStart = new vector<int>();
    _face2bc = new vector<int>();
    _bc2el = new vector<int>();
    _bc2elStart = new vector<int>();
    _bc2face = new vector<int>();
    // Métriques
    _element2Volumes = new vector<double>();
    _element2Centres = new vector<double>();
    _face2Aires = new vector<double>();
    _face2Centres = new vector<double>();
    _face2Normales = new vector<double>();

    return;
}

MeshData::~MeshData()
{
    delete _nodes;
    delete _element2Nodes_unsorted;
    delete _element2NodesStart;
    delete _elementTypes;
    // Conditions frontières
    delete _element2NodesBoundary;
    delete _element2NodesStartBoundary;
    delete _elementTypesBoundary;
    // Connectivités
    delete _esup;
    delete _esupStart;
    delete _esuel;
    delete _esuelStart;
    delete _fsuel;
    delete _esuf;
    delete _psuf;
    delete _psufStart;
    delete _face2bc;
    delete _bc2el;
    delete _bc2elStart;
    delete _bc2face;
    // Métriques
    delete _element2Volumes;
    delete _element2Centres;
    delete _face2Aires;
    delete _face2Centres;
    delete _face2Normales;

    return;
}

bool MeshData::checkClockWise(vector<int> &nodes)
{
    bool result = false;
    if (nodes.size() > 2)
    {
        vector<double> pt1 = {_nodes->at(nodes[0] * 2), _nodes->at(nodes[0] * 2 + 1)};
        vector<double> pt2 = {_nodes->at(nodes[1] * 2), _nodes->at(nodes[1] * 2 + 1)};
        vector<double> pt3 = {_nodes->at(nodes[2] * 2), _nodes->at(nodes[2] * 2 + 1)};
        double aire = 0.5 * ((pt1[0] - pt2[0]) * (pt1[1] + pt2[1]) + (pt2[0] - pt3[0]) * (pt2[1] + pt3[1]) + (pt3[0] - pt1[0]) * (pt3[1] + pt1[1]));
        if (aire < 0)
        {
            result = true;
        }
    }

    return result;
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
        _nodes->reserve(_NDIME * _NPOIN);
        _nodesReserve = true;
    }
    for (int i = 0; i < _NDIME; i++)
    {
        _nodes->push_back(stod(nodes[i]));
    }
    return;
}

void MeshData::setElement2Nodes(vector<string> element2Nodes)
{
    if (!_element2NodesReserve)
    {
        _element2Nodes_unsorted->reserve(_NELEM * (element2Nodes.size() - 2));
        _element2NodesStart->reserve(_NELEM + 1);
        _element2NodesStart->push_back(0);
        _elementTypes->reserve(_NELEM);
        _element2NodesReserve = true;
    }
    _elementTypes->push_back(stoi(element2Nodes[0]));
    for (size_t i = 1; i < element2Nodes.size() - 1; i++)
    {
        _element2Nodes_unsorted->push_back(stoi(element2Nodes[i]));
    }
    _element2NodesStart->push_back(_element2NodesStart->back() + element2Nodes.size() - 2);
    return;
}

void MeshData::setElement2NodesFrontieres()
{
    if (!_element2NodesBoundaryReserve)
    {
        _element2NodesBoundary->reserve(_NMARK);
        _element2NodesStartBoundary->reserve(_NMARK);
        _elementTypesBoundary->reserve(_NMARK);
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
        _element2NodesBoundary->push_back(vecteur_temp1);
        vector<int> vecteur_temp2;
        vecteur_temp2.reserve(_MARKER_ELEMS[marker_index - 1] + 1);
        _element2NodesStartBoundary->push_back(vecteur_temp2);
        _element2NodesStartBoundary->at(marker_index - 1).push_back(0);
        vector<int> vecteur_temp3;
        vecteur_temp3.reserve(_MARKER_ELEMS[marker_index - 1]);
        _elementTypesBoundary->push_back(vecteur_temp3);
        _element2NodesBoundaryReserves[marker_index - 1] = true;
    }
    _elementTypesBoundary->at(marker_index - 1).push_back(stoi(element2NodesFrontieres[0]));
    for (size_t i = 1; i < element2NodesFrontieres.size(); i++)
    {
        _element2NodesBoundary->at(marker_index - 1).push_back(stoi(element2NodesFrontieres[i]));
    }
    _element2NodesStartBoundary->at(marker_index - 1).push_back(_element2NodesStartBoundary->at(marker_index - 1).back() + element2NodesFrontieres.size() - 1);
    return;
}

/// Connectivité

void MeshData::setEsup()
{
    std::cout << "a°) Début de génération connectivité noeud vs elements.\n";
    // Orientation des éléments en Clock-Wise
    _element2Nodes->reserve(_element2Nodes_unsorted->size());
    for (int iElem = 0; iElem < _NELEM; iElem++)
    {
        vector<int> nodes3 = {
            _element2Nodes_unsorted->at(_element2NodesStart->at(iElem)),
            _element2Nodes_unsorted->at(_element2NodesStart->at(iElem) + 1),
            _element2Nodes_unsorted->at(_element2NodesStart->at(iElem) + 2)};
        bool check = checkClockWise(nodes3);
        if (check) // Horaire
        {
            for (int i = _element2NodesStart->at(iElem + 1) - 1; i >= _element2NodesStart->at(iElem); i--)
            {
                _element2Nodes->push_back(_element2Nodes_unsorted->at(i));
            }
        }
        else
        {
            for (int i = _element2NodesStart->at(iElem); i < _element2NodesStart->at(iElem + 1); i++)
            {
                _element2Nodes->push_back(_element2Nodes_unsorted->at(i));
            }
        }
    }

    // Initialisation de _esupStart

    _esupStart->assign(_NPOIN + 1, 0);

    // Première passe et stockage

    for (size_t i = 0; i < _element2Nodes->size(); i++)
    {
        _esupStart->at(_element2Nodes->at(i) + 1)++;
    }
    for (int i = 1; i < _NPOIN + 1; i++)
    {
        _esupStart->at(i) += _esupStart->at(i - 1);
    }

    // Deuxième passe et stockage

    _esup->assign(_esupStart->back(), 0);
    for (int iElem = 0; iElem < _NELEM; iElem++)
    {
        for (int iNode = _element2NodesStart->at(iElem); iNode < _element2NodesStart->at(iElem + 1); iNode++)
        {
            int nodeI = _element2Nodes->at(iNode);
            int istor = _esupStart->at(nodeI);
            _esupStart->at(nodeI)++;
            _esup->at(istor) = iElem;
        }
    }
    for (int iNode = _NPOIN; iNode > 0; iNode--)
    {
        _esupStart->at(iNode) = _esupStart->at(iNode - 1);
    }
    _esupStart->at(0) = 0;

    std::cout << "\t Fin génération connectivité noeud vs elements.\n";
    std::cout << "-------------------------------------------------------\n";

    return;
}

void MeshData::setEsuel()
{
    std::cout << "c°) Début de la génération de la connectivité element vs elements.\n";

    // Construction de _esuelStart
    _esuelStart->reserve(_NELEM + 1);
    _esuelStart->push_back(0);
    for (int iElem = 0; iElem < _NELEM; iElem++)
    {
        _esuelStart->push_back(VTKConnectivity::getNfael(_elementTypes->at(iElem)) + _esuelStart->back());
    }
    _NFACE = _esuelStart->back();
    _NBOUNDARY = 0;
    for (int iMark = 0; iMark < _NMARK; iMark++)
    {
        _NBOUNDARY += _MARKER_ELEMS[iMark];
    }
    _NFACE += _NBOUNDARY;
    _NFACE /= 2;

    // Construction de _esuel , _esuf et _fsuel

    _esuel->assign(_esuelStart->back(), -1);
    _fsuel->assign(_esuelStart->back(), -1);
    _esuf->assign(2 * _NFACE, -1);
    _psufStart->reserve(_NFACE + 1);
    _psufStart->push_back(0);
    _psuf->reserve(_NDIME * _NFACE);
    int iFace = 0;

    vector<int> lpoin(_NPOIN, 0);
    for (int iElem = 0; iElem < _NELEM; iElem++)
    {
        for (int iFael = 0; iFael < VTKConnectivity::getNfael(_elementTypes->at(iElem)); iFael++)
        {
            int nnofa = VTKConnectivity::getLnofa(_elementTypes->at(iElem), iFael);
            vector<int> lhelp = VTKConnectivity::getLpofa(_elementTypes->at(iElem), iFael);
            for (int i = 0; i < nnofa; i++)
            {
                lhelp[i] = _element2Nodes->at(_element2NodesStart->at(iElem) + lhelp[i]);
                lpoin[lhelp[i]] = 1;
            }
            int ipoin = lhelp[0];
            for (int istor = _esupStart->at(ipoin); istor < _esupStart->at(ipoin + 1); istor++)
            {
                int jElem = _esup->at(istor);
                if (jElem != iElem)
                {
                    for (int jFael = 0; jFael < VTKConnectivity::getNfael(_elementTypes->at(jElem)); jFael++)
                    {
                        int nnofj = VTKConnectivity::getLnofa(_elementTypes->at(jElem), jFael);
                        if (nnofj == nnofa)
                        {
                            int icoun = 0;
                            vector<int> lhelp_j = VTKConnectivity::getLpofa(_elementTypes->at(jElem), jFael);
                            for (int jnofa = 0; jnofa < nnofj; jnofa++)
                            {
                                lhelp_j[jnofa] = _element2Nodes->at(_element2NodesStart->at(jElem) + lhelp_j[jnofa]);
                                icoun += lpoin[lhelp_j[jnofa]];
                            }
                            if (icoun == nnofa)
                            {
                                _esuel->at(_esuelStart->at(iElem) + iFael) = jElem;

                                if (_esuel->at(_esuelStart->at(jElem) + jFael) == iElem)
                                {
                                    _fsuel->at(_esuelStart->at(iElem) + iFael) = _fsuel->at(_esuelStart->at(jElem) + jFael);
                                }
                                else
                                {
                                    // Adding a new face
                                    _fsuel->at(_esuelStart->at(iElem) + iFael) = iFace;
                                    _fsuel->at(_esuelStart->at(jElem) + jFael) = iFace;
                                    _esuf->at(iFace * 2 + 0) = iElem;
                                    _esuf->at(iFace * 2 + 1) = jElem;
                                    _psufStart->push_back(_psufStart->back() + nnofa);
                                    for (int inofa = 0; inofa < nnofa; inofa++)
                                    {
                                        _psuf->push_back(lhelp[inofa]);
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
    std::cout << "\tFin de la génération de la connectivité element vs elements.\n";
    std::cout << "-------------------------------------------------------------------\n";

    return;
}

void MeshData::setGhostCell()
{
    int iFace = _NFACE - _NBOUNDARY;
    int nelem = _NELEM;
    _face2bc->reserve(2 * _NBOUNDARY);
    for (int iElem = 0; iElem < _NELEM; iElem++)
    {
        for (int j = _esuelStart->at(iElem); j < _esuelStart->at(iElem + 1); j++)
        {
            if (_esuel->at(j) == -1)
            {
                int iFael = j - _esuelStart->at(iElem);
                _esuel->at(j) = nelem;
                _esuf->at(iFace * 2 + 0) = iElem;
                _esuf->at(iFace * 2 + 1) = nelem;
                _fsuel->at(_esuelStart->at(iElem) + iFael) = iFace;
                int nnofa = VTKConnectivity::getLnofa(_elementTypes->at(iElem), iFael);
                _psufStart->push_back(_psufStart->back() + nnofa);
                vector<int> lhelp = VTKConnectivity::getLpofa(_elementTypes->at(iElem), iFael);
                for (int i = 0; i < nnofa; i++)
                {
                    lhelp[i] = _element2Nodes->at(_element2NodesStart->at(iElem) + lhelp[i]);
                    _psuf->push_back(lhelp[i]);
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
                            _face2bc->push_back(iFace);
                            _face2bc->push_back(iMark);
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

void MeshData::setBC()
{
    _bc2el->reserve(2 * _NBOUNDARY);
    _bc2face->reserve(_NBOUNDARY);
    _bc2elStart->reserve(_NMARK + 1);
    _bc2elStart->push_back(0);
    for (int iMark = 0; iMark < _NMARK; iMark++)
    {
        _bc2elStart->push_back(_bc2elStart->back() + _MARKER_ELEMS[iMark]);
        for (int iBoundary = 0; iBoundary < _NBOUNDARY; iBoundary++)
        {
            if (_face2bc->at(2 * iBoundary + 1) == iMark)
            {
                int iFace = _face2bc->at(2 * iBoundary);
                _bc2el->push_back(_esuf->at(2 * iFace + 0));
                _bc2el->push_back(_esuf->at(2 * iFace + 1));
                _bc2face->push_back(iFace);
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
    setBC();
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

vector<double> *MeshData::getNodes() const
{
    return _nodes;
}

vector<int> *MeshData::getElement2Nodes() const
{
    return _element2Nodes;
}

void MeshData::getElement2Nodes(int &iElem, vector<int> &nodes) const
{
    for (int iNode = _element2NodesStart->at(iElem); iNode < _element2NodesStart->at(iElem + 1); iNode++)
    {
        nodes.push_back(_element2Nodes->at(iNode));
    }

    return;
}

vector<int> *MeshData::getElement2NodesStart() const
{
    return _element2NodesStart;
}

vector<int> *MeshData::getElementTypes() const
{
    return _elementTypes;
}

vector<vector<int>> *MeshData::getElement2NodesBoundary() const
{
    return _element2NodesBoundary;
}

vector<vector<int>> *MeshData::getElement2NodesStartBoundary() const
{
    return _element2NodesStartBoundary;
}

vector<vector<int>> *MeshData::getElementTypesBoundary() const
{
    return _elementTypesBoundary;
}

void MeshData::getElement2NodesBoundary(int iMark, int iFael, vector<int> &lhelp)
{
    for (int i = _element2NodesStartBoundary->at(iMark)[iFael]; i < _element2NodesStartBoundary->at(iMark)[iFael + 1]; i++)
    {
        lhelp.push_back(_element2NodesBoundary->at(iMark)[i]);
    }
    std::sort(lhelp.begin(), lhelp.end());

    return;
}

/// Connectivité

vector<int> *MeshData::getEsup() const
{
    return _esup;
}

vector<int> *MeshData::getEsupStart() const
{
    return _esupStart;
}

int MeshData::getVTKindex(const int &iElem)
{
    return _elementTypes->at(iElem);
}

int MeshData::getNfael(const int &iElem)
{
    return VTKConnectivity::getNfael(_elementTypes->at(iElem));
}

int MeshData::getLnofa(const int &iElem, const int &iFael)
{
    return VTKConnectivity::getLnofa(_elementTypes->at(iElem), iFael);
}

vector<int> MeshData::getLpofa(const int &iElem, const int &iFael)
{
    return VTKConnectivity::getLpofa(_elementTypes->at(iElem), iFael);
}

void MeshData::getFace2Nodes(int &iFace, vector<int> &nodes) const
{
    for (int iNode = _psufStart->at(iFace); iNode < _psufStart->at(iFace + 1); iNode++)
    {
        nodes.push_back(_psuf->at(iNode));
    }

    return;
}

vector<int> *MeshData::getEsuel() const
{
    return _esuel;
}

vector<int> *MeshData::getEsuelStart() const
{
    return _esuelStart;
}

vector<int> *MeshData::getFsuel() const
{
    return _fsuel;
}

vector<int> *MeshData::getEsuf() const
{
    return _esuf;
}

vector<int> *MeshData::getPsuf() const
{
    return _psuf;
}

vector<int> *MeshData::getPsufStart() const
{
    return _psufStart;
}

vector<int> *MeshData::getFace2bc() const
{
    return _face2bc;
}

vector<int> *MeshData::getBc2el() const
{
    return _bc2el;
}

vector<int> *MeshData::getBc2elStart() const
{
    return _bc2elStart;
}
vector<int> *MeshData::getBc2face() const
{
    return _bc2face;
}

/////////////////////////////////////////////////////////////////////////////////////

/////////////////////    METRIQUES        ///////////////////////////////////////////

void MeshData::initializeMetric()
{
    _element2Volumes->reserve(_NELEM);
    _element2Centres->reserve(_NELEM * _NDIME);
    _face2Aires->reserve(_NFACE);
    _face2Centres->reserve(_NFACE * _NDIME);
    _face2Normales->reserve(_NFACE * _NDIME);
    return;
}

void MeshData::setElement2Volumes(const double &volume)
{
    _element2Volumes->push_back(volume);
    return;
}

void MeshData::setElement2Centres(const vector<double> &centre)
{
    for (size_t i = 0; i < centre.size(); i++)
    {
        _element2Centres->push_back(centre[i]);
    }
    return;
}

void MeshData::setFace2Aires(const double &aire)
{
    _face2Aires->push_back(aire);
    return;
}

void MeshData::setFace2Centres(const vector<double> &centre)
{
    for (size_t i = 0; i < centre.size(); i++)
    {
        _face2Centres->push_back(centre[i]);
    }

    return;
}

void MeshData::setFace2Normales(const vector<double> &normale)
{
    for (size_t i = 0; i < normale.size(); i++)
    {
        _face2Normales->push_back(normale[i]);
    }
    return;
}

vector<double> *MeshData::getElement2Volumes() const
{
    return _element2Volumes;
}

vector<double> *MeshData::getElement2Centres() const
{
    return _element2Centres;
}

vector<double> *MeshData::getFace2Aires() const
{
    return _face2Aires;
}

vector<double> *MeshData::getFace2Centres() const
{
    return _face2Centres;
}

vector<double> *MeshData::getFace2Normales() const
{
    return _face2Normales;
}