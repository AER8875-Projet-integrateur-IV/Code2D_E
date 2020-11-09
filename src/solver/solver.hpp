/* 
/Fichier solver.hpp
/Date: 05 novembre 2020 - 02h39
/Projet: Projet Intégrateur 4
*/
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <math.h>

#include "../mesh/meshData/meshData.hpp"
#include "../inputData.hpp"
#include "./solutionStruct.hpp"

using namespace std;

struct Properties
{
    double Ma;
    double AOA;
    double CFL;
    double c;
    double gamma;
};

struct MeshDimension
{
    int NELEM;
    int NPOIN;
    int NFACE;
    int NBOUNDARY;
    int NMARK;
};

class Solver
{
private:
    MeshData *_meshData;
    InputData *_inputData;
    Solution W;
    Solution dW;
    Properties props;
    MeshDimension meshDim;

    // Conditions limites
    vector<int> _conditionsLimites;
    vector<int> *_bc2el;
    vector<int> *_bc2elStart;

    // Métriques
    vector<double> *_element2Volumes;
    vector<double> *_element2Centres;
    vector<double> *_face2Aires;
    vector<double> *_face2Centres;
    vector<double> *_face2Normales;

    void initializeSolver();
    void initializeSolution();
    void updateBoundaryCells();
    void computeTimeSteps();

public:
    Solver(MeshData *meshData, InputData *inputData);
    ~Solver();

    // Setters
};