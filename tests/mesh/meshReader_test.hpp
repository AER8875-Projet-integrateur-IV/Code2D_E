#pragma once

#include "../../src/mesh/meshData/meshData.hpp"
#include "../../src/mesh/meshReader/meshRead.hpp"
//#include "../../src/mesh/meshReader/su2Reader.hpp"

void openFile_test(MeshRead meshRead);
void verification_lectureMaillage(MeshRead meshRead, shared_ptr<MeshData> meshData);

void main_meshReader_Test();