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
    shared_ptr<MeshData> _meshData;
    FILE *_inputFile;

    bool isFileValid();

public:
    Su2Reader(string &path, shared_ptr<MeshData> meshData);
    ~Su2Reader();
    void readFile();
};