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

class Solver
{
private:
    shared_ptr<MeshData> _meshData;
    shared_ptr<InputData> _inputData;
    Solution W;
    Solution dW;
    Properties props;

    void initializeSolver();
    void initializeSolution();

public:
    Solver(shared_ptr<MeshData> meshData, shared_ptr<InputData> inputData);
    ~Solver();

    // Setters
};