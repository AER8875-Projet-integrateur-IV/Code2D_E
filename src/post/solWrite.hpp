
/*
/Fichier solWrite.hpp
/Date: octobre 2020
/Projet: Projet Intégrateur 4
*/

#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>

#include "../mesh/meshData/meshData.hpp"
#include "./VtuWriter.hpp"
#include "./TecWriter.hpp"
#include "../../tools/stringTools.hpp"
#include "../solver/solver.hpp"
#include "../inputData.hpp"

using namespace std;

class SolWrite : public StringTools
{
private:
    string _path;
    MeshData *_meshData;
    Solver *_solver;
    InputData *_inputData;
    void writeErrors();

public:
    SolWrite(string &path, MeshData *meshData, InputData *inputData, Solver *solver);
    ~SolWrite();
    void writeFile();
};