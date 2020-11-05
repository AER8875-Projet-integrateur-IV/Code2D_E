/*
/Fichier VtuWriter.cpp
/Date: octobre 2020
/Projet: Projet Intégrateur 4
*/


#include "./VtuWriter.hpp"

using std::ofstream;

VtuWriter::VtuWriter(string &format, shared_ptr<MeshData> meshData)
{
    _format = format;
    _meshData = meshData;
    _outputFile = NULL;
    // cout << "Vtu file start" << endl; follow1
}

///////////Validation du fichier
VtuWriter::~VtuWriter()
{
    if (_outputFile != NULL)
    {
        fclose(_outputFile);
    }
    return;
}

void VtuWriter::writeFile()
{
  ///////////Suite de void qui ecrivent le fichier*
  ofstream fileStream(_format);
  beginFile(fileStream);
  writePoints(fileStream);
  writeCells(fileStream);
  writePointsData(fileStream);
  writeCellsData(fileStream);
  endFile (fileStream);
  fileStream.close();
  cout << "VtuWriter is working3!" << endl;
}

///////////Début du fichier
void VtuWriter::beginFile(ofstream &fileStream)
{
     fileStream << "<VTKFile type=\"UnstructuredGrid\">"
                << "\n"
                << "<UnstructuredGrid>"
                << "\n"
                << "<Piece NumberOfPoints=\"" << _meshData->getNPOIN() << "\" NumberOfCells=\" " << _meshData->getNELEM() << "\">"
                << "\n";
}

///////////Ecriture des coordonnees des points
void VtuWriter::writePoints(ofstream &fileStream)
{
  	fileStream << "<Points>"
  	           << "<DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\">"
  	           << "\n";
  	uint32_t returnline = 0;
  	for (returnline = 0; returnline < unsigned(_meshData->getNPOIN()) ; returnline=returnline+1)
     {
  		fileStream << _meshData->getNodes()[2*returnline] << " " << _meshData->getNodes()[2*returnline+1] << " "
  		           << "0.0"
  		           << "\n";
    //_meshData->getNodes()[2*returnline+2] pour ndim=3
  	}
  	fileStream << "</DataArray>"
  	           << "\n"
  	           << "</Points>";
}

///////////Ecriture des coordonnees des cellules
void VtuWriter::writeCells(ofstream &fileStream)
{
	fileStream << "<Cells>"
	           << "\n"
	           << " <DataArray type=\"UInt32\" Name=\"connectivity\" format=\"ascii\">"
	           << "\n";
	uint32_t returnline = 0;

///////////Ecriture des noeuds pour les elements
  for (returnline = 0; returnline < unsigned(_meshData->getNELEM())*4 ; returnline=returnline+4)
  {
		fileStream << _meshData->getElement2Nodes()[returnline] << " " << _meshData->getElement2Nodes()[returnline+1]<< " " << _meshData->getElement2Nodes()[returnline+2]<< " " << _meshData->getElement2Nodes()[returnline+3];
		fileStream << "\n";
	}

  fileStream << "</DataArray>"
             << "\n"
             << "<DataArray type=\"UInt32\" Name=\"offsets\" format=\"ascii\">"
             << "\n";

  returnline = 0;
            //Ecriture des positions de depart des noeuds
            for(returnline = 0; returnline < unsigned(_meshData->getNELEM()) ; returnline=returnline+1)
            {
              fileStream << _meshData->getElement2NodesStart()[returnline] << " ";
            }
            fileStream << "\n";

  returnline = 0;

  fileStream << "</DataArray>"
             << "\n"
             << "<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">"
             << "\n";
             //Ecriture des types de geometries VTK
            for (returnline = 0; returnline < unsigned(_meshData->getNELEM()) ; returnline=returnline+1)
            {
                fileStream << _meshData->getVTKindex(returnline) << " ";
                fileStream << "\n";
            }

  fileStream << "</DataArray>"
             << "\n"
             << "</Cells>"
             << "\n";
  }

  void VtuWriter::writePointsData(ofstream &fileStream)
  {
    std::cout << "Check" << '\n';
  }

  void VtuWriter::writeCellsData(ofstream &fileStream)
  {
    // Ecriture des Pressions
    fileStream << "<CellData Scalars=\"Pression\" Vectors=\"velocity\" >"
  	           << "\n"
  	           << "<DataArray type=\"Float64\" Name=\"Pression\" format=\"ascii\" >"
  	           << "\n";



  	fileStream << "</DataArray>"
  	           << "\n";

  	 // Ecriture des vitesses
  	fileStream << "<DataArray type=\"Float64\" Name=\"velocity\" format=\"ascii\" NumberOfComponents=\"3\" >"
               << "\n";



    fileStream << "</DataArray>"
               << "\n"
               << "</CellData>"
               << "\n";
    std::cout << "Mate" << '\n';

  }


  ///////////Fin du fichier
  void VtuWriter::endFile(ofstream &fileStream) {
  	fileStream << "</Piece>"
  	           << "\n"
  	           << "</UnstructuredGrid>"
  	           << "\n"
  	           << "</VTKFile>";
  }
