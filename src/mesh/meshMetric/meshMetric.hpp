/* 
/Fichier meshMetric.hpp
/Date: 21 octobre 2020 - 01h13
/Projet: Projet Intégrateur 4
*/
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

#include "../meshData/meshData.hpp"

using namespace std;

class MeshMetric
{
private:
    shared_ptr<MeshData> _meshData;

public:
    MeshMetric(shared_ptr<MeshData> meshData);
    ~MeshMetric();

    // Setters

    // Getters
};