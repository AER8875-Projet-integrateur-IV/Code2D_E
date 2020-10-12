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
/* #include "./su2Reader.hpp"
#include "../../tools/stringTools.hpp" */

using namespace std;

class MeshConnectivity
{
private:
    shared_ptr<MeshData> _meshData;

public:
    MeshConnectivity(shared_ptr<MeshData> meshData);
    ~MeshConnectivity();
};