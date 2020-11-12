/* 
/Fichier meshConnectivity_test.hpp
/Date: 12 octobre 2020 - 20h56
/Projet: Projet Intégrateur 4
*/
#pragma once

#include <memory>
#include <vector>

#include "../../src/mesh/meshData/meshData.hpp"
#include "../../src/mesh/meshData/vtkConnectivity.hpp"
#include "../../src/mesh/meshReader/meshRead.hpp"

using namespace std;

void connectiviteNodeElements(MeshData *meshData);
void connectiviteFacesLocale(MeshData *meshData);
void connectiviteElementElements(MeshData *meshData);
void connectiviteFaces(MeshData *meshData);
void connectiviteBoundary(MeshData *meshData);

void main_meshConnectivity_Test();