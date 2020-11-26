/*
/Fichier TecWriter.cpp
/Date: novembre 2020
/Projet: Projet Intégrateur 4
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "../mesh/meshData/meshData.hpp"
#include "../../tools/stringTools.hpp"
#include "../solver/solver.hpp"

using namespace std;

class TecWriter
{
private:
    string _path;
    MeshData *_meshData;
    Solver *_solver;
    Solution *_solution;
    FILE *_outputFile;

    bool isFileValid();

public:
    TecWriter(string &path, MeshData *meshData, Solver *solver);
    ~TecWriter();
    void writeFile();
    void beginFile(std::ofstream &);
    void writeNewZone(std::ofstream &);
    void writeCoord(std::ofstream &);
    void writeVar(std::ofstream &);
    void writeElementConnectivity(ofstream &fileStream);
};
