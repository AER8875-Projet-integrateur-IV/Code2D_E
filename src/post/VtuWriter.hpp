
/*
/Fichier VtuWriter.hpp
/Date: octobre 2020
/Projet: Projet Intégrateur 4
*/

/*
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "../mesh/meshData/meshData.hpp"
#include "../../tools/stringTools.hpp"

using namespace std;

class VtuWriter
{
private:
    string _format;
    shared_ptr<MeshData> _meshData;
    FILE *_outputFile;

    bool isFileValid();
/* bool setIndice(string ligne, int &indice, int &marker_index); */
/*       void setElement2Nodes(string ligne); */
/*       void setElement2NodesFrontieres(string ligne, int marker_index); */
/*       void setNodes(string ligne); 

public:
VtuWriter(string &format, shared_ptr<MeshData> meshData);
~VtuWriter();
void writeFile();
void beginFile(std::ofstream &);
void writePoints(std::ofstream &);
void writeCells(std::ofstream &);
void writePointsData(std::ofstream &);
void writeCellsData(std::ofstream &);
void endFile(std::ofstream &);
};

*/
