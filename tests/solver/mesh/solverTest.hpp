/* 
/Fichier meshConnectivity_test.hpp
/Date: 12 octobre 2020 - 20h56
/Projet: Projet Intégrateur 4
*/
#pragma once

#include <memory>
#include <vector>

#include "../../src/mesh/meshData/meshData.hpp"
#include "../../src/mesh/meshReader/meshRead.hpp"
#include "../../src/mesh/meshMetric/meshMetric.hpp"
#include "../../src/inputData.hpp"
#include "../../src/solver/solver.hpp"

using namespace std;
class SolverTest
{
private:
    MeshData *_meshData;
    InputData *_inputData;
    Solver *_solver;

public:
    SolverTest(MeshData *meshData, InputData *inputData, Solver *_olver);
    ~SolverTest();
    void startSolver();
    void initializeSolver();
    void makeIterationSolver();
};

void main_solver_Test();