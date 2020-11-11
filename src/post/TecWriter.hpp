/*
/Fichier TecWriter.cpp
/Date: novembre 2020
/Projet: Projet Intégrateur 4
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "../mesh/meshData/meshData.hpp"
#include "../../tools/stringTools.hpp"

using namespace std;

class TecWriter
{
private:
    string _format;
    shared_ptr<MeshData> _meshData;
    FILE *_outputFile;

    bool isFileValid();
/* bool setIndice(string ligne, int &indice, int &marker_index); */
/*       void setElement2Nodes(string ligne); */
/*       void setElement2NodesFrontieres(string ligne, int marker_index); */
/*       void setNodes(string ligne); */

public:
    TecWriter(string &format, shared_ptr<MeshData> meshData);
    ~TecWriter();
    void writeFile();
    void beginFile(std::ofstream&);
    void writeNewZone(std::ofstream&);
    void writeCoord(std::ofstream&);
    void writeVar(std::ofstream&);
    void writeFaceConnectivity(std::ofstream&);
    // void writeCellsData(std::ofstream&);
    // void endFile(std::ofstream&);
};
