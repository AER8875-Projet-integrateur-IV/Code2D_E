/* 
/Fichier solver.hpp
/Date: 05 novembre 2020 - 02h39
/Projet: Projet Intégrateur 4
*/
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include "../mesh/meshData/meshData.hpp"
#include "../inputData.hpp"
#include "./solutionStruct.hpp"
#include "./schemes/schemes.hpp"

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
    InputData *_inputData;
    Properties _props;
    MeshData *_meshData;
    MeshDimension _meshDim;
    Schemes *_schemes;
    // Paramètres du solveurs
    Solution *_W;
    Solution *_dW;
    Flux *_F;
    vector<double> *_timeSteps;
    Residual *_R;

    // Conditions limites
    vector<int> _conditionsLimites;

    // Connectivité
    vector<int> *_element2Nodes;
    vector<int> *_element2NodesStart;
    vector<vector<int>> *_element2NodesBoundary;
    vector<vector<int>> *_element2NodesStartBoundary;
    vector<int> *_esup;
    vector<int> *_esupStart;
    vector<int> *_esuelStart;
    vector<int> *_esuel;
    vector<int> *_fsuel;
    vector<int> *_esuf;
    vector<int> *_psuf;
    vector<int> *_psufStart;
    vector<int> *_face2bc;
    vector<int> *_bc2el;
    vector<int> *_bc2elStart;
    vector<int> *_bc2face;
    // Métriques
    vector<double> *_element2Volumes;
    vector<double> *_element2Centres;
    vector<double> *_face2Aires;
    vector<double> *_face2Centres;
    vector<double> *_face2Normales;
    vector<double> *_CVprojections;

    // Méthodes internes
    void computeEnergie(Solution *solution, int &index);
    void computeVn(const Solution *solution, int &iCell, int &iFace, double &Vn);

    void initializeSolver();
    void initializeSolution();
    void initializeEachIteration();
    void updateBoundaryCells();
    void computeTimeSteps();
    void computeResiduals();
    void updateW();

public:
    Solver(MeshData *meshData, InputData *inputData);
    ~Solver();

    // Setters
};