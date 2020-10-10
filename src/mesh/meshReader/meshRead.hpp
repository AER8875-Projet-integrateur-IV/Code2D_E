#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../meshData/meshData.hpp"

using namespace std;

class MeshRead
{
private:
    string _path;
    MeshData *_meshData;

    vector<vector<double> *> *element2nodesBoundary;
    vector<int *> *element2nodesStartBoundary;
    vector<int *> *boundaryTypes;

public:
    MeshRead(string &path, MeshData *meshData);
    ~MeshRead();
    void read();
};