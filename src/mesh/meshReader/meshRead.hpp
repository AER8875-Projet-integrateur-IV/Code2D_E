#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../meshData/meshData.hpp"
#include "./su2Reader.hpp"
#include "../../tools/stringTools.cpp"

using namespace std;

class MeshRead
{
private:
    string _path;
    shared_ptr<MeshData> _meshData;

public:
    MeshRead(string &path, shared_ptr<MeshData> meshData);
    ~MeshRead();
    void readFile();
};