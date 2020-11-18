
/*
/Fichier solWrite.hpp
/Date: octobre 2020
/Projet: Projet Intégrateur 4
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../mesh/meshData/meshData.hpp"
#include "./VtuWriter.hpp"
#include "./TecWriter.hpp"
#include "../../tools/stringTools.hpp"
#include "../solver/solutionStruct.hpp"

using namespace std;

class SolWrite : public StringTools
{
private:
    string _path;
    MeshData *_meshData;
//    Solution *_solution;

public:
    SolWrite(string &path, MeshData *meshData);
    ~SolWrite();
    void writeFile();
};
