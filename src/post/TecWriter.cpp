/*
/Fichier TecWriter.cpp
/Date: novembre 2020
/Projet: Projet Intégrateur 4
*/


#include "./TecWriter.hpp"

using std::ofstream;

TecWriter::TecWriter(string &format, shared_ptr<MeshData> meshData)
{
    _format = format;
    _meshData = meshData;
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
  ofstream fileStream(_format);
  beginFile(fileStream);
  writeNewZone(fileStream);
  writeCoord(fileStream);
  writeVar(fileStream);
  writeFaceConnectivity(fileStream);
  fileStream.close();
 }

///////////Début du fichier
void TecWriter::beginFile(ofstream &fileStream)
{
  fileStream << "TITLE = \"Exemple\"\nVARIABLES = \"X\",\"Y\"" << endl;
}

void TecWriter::writeNewZone(ofstream &fileStream)
{
  fileStream << "ZONE "
           << "ZONETYPE=FEPOLYGON "
           << "NODES=" << _meshData->getNPOIN() << ", "
           << "ELEMENTS=" << _meshData->getNELEM() << ", "
           << "FACES=" << _meshData->getNFACE() << ", "
           << "NUMCONNECTEDBOUNDARYFACES=0, TOTALNUMBOUNDARYCONNECTIONS=0\n"
           << "DATAPACKING=BLOCK";
}

///////////Ecriture des coordonnees des points
void TecWriter::writeCoord(ofstream &fileStream)
{

    uint32_t returnline = 0;
    for (returnline = 0; returnline < unsigned(_meshData->getNPOIN()) ; returnline=returnline+1)
         {
       		fileStream << _meshData->getNodes()[2*returnline] << "\t" << _meshData->getNodes()[2*returnline+1] << "\t"
       		           << "0.0"
       		           << "\n";
         //_meshData->getNodes()[2*returnline+2] pour ndim=3
       	}
}

///////////Ecriture des variables
void TecWriter::writeVar(ofstream &fileStream)
{
    // cout << "Tec file start4" << endl;
}

///////////Ecriture des Connectivités
void TecWriter::writeFaceConnectivity(ofstream &fileStream)
{
  int returnline = 0;
  for (returnline = 0; returnline < _meshData->getNFACE() ; returnline=returnline+1)
      {
           vector<int> nodes;
          _meshData->getFace2Nodes(returnline, nodes);
          for (size_t i = 0; i < nodes.size(); i++)
          {
              fileStream << nodes[i] << "\t";
          }
        fileStream << "\n";
      }
  returnline = 0;
  for (returnline = 0; returnline < _meshData->getNFACE() ; returnline=returnline+1)
      {
            fileStream << _meshData->getEsuf()[2*returnline] << "\t"<< _meshData->getEsuf()[(2*returnline)+1];
            fileStream << "\n";
      }
  }
