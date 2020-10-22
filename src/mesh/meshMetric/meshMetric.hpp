/* 
/Fichier meshMetric.hpp
/Date: 21 octobre 2020 - 02h13
/Projet: Projet Intégrateur 4
*/
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <math.h>

#include "../meshData/meshData.hpp"

using namespace std;

class MeshMetric
{
private:
    shared_ptr<MeshData> _meshData;
    vector<double> _nodes;
    int _NDIME;

    void calculAireTriangle(vector<double> &pt1, vector<double> &pt2, vector<double> &pt3, double &aire, vector<double> &centre);
    void calculTriangle(vector<int> &nodes, double &aire, vector<double> &centre);
    void calculQuad(vector<int> &nodes, double &aire, vector<double> &centre);
    void setElements();
    void setFaces();

public:
    MeshMetric(shared_ptr<MeshData> meshData);
    ~MeshMetric();

    // Setters

    void setMetric();

    // Getters
};