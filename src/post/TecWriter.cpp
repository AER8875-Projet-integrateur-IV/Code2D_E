/*
/Fichier TecWriter.cpp
/Date: novembre 2020
/Projet: Projet Intégrateur 4
*/

#include "./TecWriter.hpp"

using std::ofstream;

TecWriter::TecWriter(string &path, MeshData *meshData, Solver *solver)
{
  _path = path;
  _meshData = meshData;
  _solver = solver;
  _solution = _solver->getSolution();
  _outputFile = NULL;
}

///////////Validation du fichier
TecWriter::~TecWriter()
{
  if (_outputFile != NULL)
  {
    fclose(_outputFile);
  }
  return;
}

void TecWriter::writeFile()
{
  /////////Suite de void qui ecrivent le fichier*
  ofstream fileStream(_path);
  beginFile(fileStream);
  writeNewZone(fileStream);
  writeCoord(fileStream);
  // writeVar(fileStream);
  writeFaceConnectivity(fileStream);
  //writeElementConnectivity(fileStream);
  fileStream.close();
}

///////////Début du fichier
void TecWriter::beginFile(ofstream &fileStream)
{
  fileStream << "TITLE = \"Exemple\"\nVARIABLES = \"X\",\"Y\",\"Densité\",\"Vitesse U\",\"Vitesse V\", \"Pression\",\"Énergie\",\"Rrho\"" << endl;
}

void TecWriter::writeNewZone(ofstream &fileStream)
{
  fileStream << "ZONE "
             << "ZONETYPE=FEPOLYGON "
             << "NODES=" << _meshData->getNPOIN() << ", "
             << "ELEMENTS=" << _meshData->getNELEM() << ", "
             << "FACES=" << _meshData->getNFACE() << ", "
             << "NUMCONNECTEDBOUNDARYFACES=0, TOTALNUMBOUNDARYCONNECTIONS=0\n"
             << "DATAPACKING=BLOCK, VARLOCATION=([3-8]=CELLCENTERED)\n";
}

///////////Ecriture des coordonnees des points
void TecWriter::writeCoord(ofstream &fileStream)
{

  uint32_t returnline = 0;
  for (returnline = 0; returnline < unsigned(_meshData->getNPOIN()); returnline = returnline + 1)
  {
    fileStream << _meshData->getNodes()->at(2 * returnline) << "\n";
  }
  for (returnline = 0; returnline < unsigned(_meshData->getNPOIN()); returnline = returnline + 1)
  {
    fileStream << _meshData->getNodes()->at(2 * returnline + 1) << "\n";
  }
}

///////////Ecriture des variables
void TecWriter::writeVar(ofstream &fileStream)
{
  for (int iElem = 0; iElem < _meshData->getNELEM(); iElem++)
  {
    fileStream << _solution->rho[iElem] << "\n";
  }
  for (int iElem = 0; iElem < _meshData->getNELEM(); iElem++)
  {
    fileStream << _solution->rhoU[iElem] / _solution->rho[iElem] << "\n";
  }
  for (int iElem = 0; iElem < _meshData->getNELEM(); iElem++)
  {
    fileStream << _solution->rhoV[iElem] / _solution->rho[iElem] << "\n";
  }
  for (int iElem = 0; iElem < _meshData->getNELEM(); iElem++)
  {
    fileStream << _solution->rhoE[iElem] / _solution->rho[iElem] << "\n";
  }
  for (int iElem = 0; iElem < _meshData->getNELEM(); iElem++)
  {
    fileStream << _solution->p[iElem] << "\n";
  }
  for (int iElem = 0; iElem < _meshData->getNELEM(); iElem++)
  {
    fileStream << _solver->getResidus()->rhoVds[iElem] << "\n";
  }
}

///////////Ecriture des Connectivités
void TecWriter::writeFaceConnectivity(ofstream &fileStream)
{
  // Face2Nodes connectivity
  for (int iFace = 0; iFace < _meshData->getNFACE(); iFace++)
  {
    vector<int> n;
    _meshData->getFace2Nodes(iFace, n);
    for (int i = 0; i < n.size(); i++)
    {
      fileStream << n[i] << "\t";
    }
    fileStream << "\n";
  }

  //Left and right elements for each face
  uint32_t returnline = 0;
  for (returnline = 0; returnline < unsigned(_meshData->getNFACE()); returnline++)
  {
    fileStream << _meshData->getEsuf()->at(2 * returnline) << "\t";
  }
  fileStream << "\n";
  for (returnline = 0; returnline < unsigned(_meshData->getNFACE()); returnline++)
  {
    fileStream << _meshData->getEsuf()->at(2 * returnline + 1) << "\t";
  }
}
// void TecWriter::writeElementConnectivity(ofstream &fileStream)
// {
//   for (int iElem = 0; iElem < _meshData->getNELEM(); iElem++)
//   {
//     for (int jNode = _meshData->getElement2NodesStart()->at(iElem); jNode < _meshData->getElement2NodesStart()->at(iElem + 1); jNode++)
//     {
//       fileStream << _meshData->getElement2Nodes()->at(jNode) + 1 << "\t";
//     }
//     fileStream << "\n";
//   }
// }
