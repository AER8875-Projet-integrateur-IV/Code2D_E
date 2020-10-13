/* 
/Fichier meshConnectivity_test.hpp
/Date: 12 octobre 2020 - 20h56
/Projet: Projet Intégrateur 4
*/
#pragma once

#include <memory>
#include <vector>

#include "../../src/mesh/meshData/meshData.hpp"
#include "../../src/mesh/meshReader/meshRead.hpp"

using namespace std;

void connectiviteNodeElements(shared_ptr<MeshData> meshData);
void connectiviteFaces(shared_ptr<MeshData> meshData);
void connectiviteElementElements(shared_ptr<MeshData> meshData);

void main_meshConnectivity_Test();