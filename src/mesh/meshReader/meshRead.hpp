/* 
/Fichier meshRead.hpp
/Date: octobre 2020 
/Projet: Projet Intégrateur 4
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../meshData/meshData.hpp"
#include "./su2Reader.hpp"
#include "../../tools/stringTools.hpp"

using namespace std;

class MeshRead : public StringTools
{
private:
    string _path;
    MeshData *_meshData;

public:
    MeshRead(string &path, MeshData *meshData);
    ~MeshRead();
    void readFile();
};