/* 
/Fichier meshMetric_test.hpp
/Date: 21 octobre 2020 - 04h18
/Projet: Projet Intégrateur 4
*/
#pragma once

#include <memory>
#include <vector>

#include "../../src/mesh/meshData/meshData.hpp"
#include "../../src/mesh/meshMetric/meshMetric.hpp"
#include "../../src/mesh/meshReader/meshRead.hpp"

using namespace std;

void metricElements(shared_ptr<MeshData> meshData);
void metricFaces(shared_ptr<MeshData> meshData);

void main_meshMetric_Test();