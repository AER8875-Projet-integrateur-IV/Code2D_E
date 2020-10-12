/* 
/Fichier meshConnectivity.hpp
/Date: 12 octobre 2020 - 19h33
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
    shared_ptr<MeshData> _meshData;

public:
    MeshRead(string &path, shared_ptr<MeshData> meshData);
    ~MeshRead();
    void readFile();
};