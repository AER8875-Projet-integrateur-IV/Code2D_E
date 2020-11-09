/* 
/Fichier su2Reader.hpp
/Date: octobre 2020 
/Projet: Projet Intégrateur 4
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../meshData/meshData.hpp"
#include "../../tools/stringTools.hpp"

using namespace std;

class Su2Reader : public StringTools
{
private:
    string _path;
    MeshData *_meshData;
    FILE *_inputFile;

    bool isFileValid();
    bool setIndice(string ligne, int &indice, int &marker_index);
    void setParametres(string ligne, int indice, int marker_index);
    void setElement2Nodes(string ligne);
    void setElement2NodesFrontieres(string ligne, int marker_index);
    void setNodes(string ligne);

public:
    Su2Reader(string &path, MeshData *meshData);
    ~Su2Reader();
    void readFile();
};
