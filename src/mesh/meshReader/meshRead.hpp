#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../meshData/meshData.hpp"
#include "./su2Reader.hpp"
#include "../../tools/stringTools.hpp"

using namespace std;

class MeshRead
{
private:
    string _path;
    MeshData *_meshData;
    Su2Reader _su2Reader;

    vector<vector<double> *> *element2nodesBoundary;
    vector<int *> *element2nodesStartBoundary;
    vector<int *> *boundaryTypes;

public:
    MeshRead(string &path, MeshData *meshData);
    ~MeshRead();
    void readFile();
};