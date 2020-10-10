#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../meshData/meshData.hpp"
using namespace std;

class Su2Reader
{
private:
    string _path;
    MeshData *_meshData;

public:
    Su2Reader(string &path, MeshData *meshData);
    ~Su2Reader();
    void read();
};