/* 
/Fichier meshConnectivity_test.cpp
/Date: 12 octobre 2020 - 20h56
/Projet: Projet Intégrateur 4
*/
#include "./solverTest.hpp"

SolverTest::SolverTest(MeshData *meshData, InputData *inputData, Solver *solver)
{
    _solver = solver;
    _meshData = meshData;
    _inputData = inputData;
    return;
}
SolverTest::~SolverTest()
{
    return;
}

void SolverTest::initializeSolver() // Etat : Succés
{
    _solver->runSolver();
    return;
}

void main_solver_Test()
{
    MeshData *meshData = new MeshData();
    string pathInput = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/inputData/Example.project";
    InputData *inputData = new InputData(pathInput);
    inputData->readFile();
    string meshPath = inputData->getMeshPath();
    MeshRead meshRead = MeshRead(meshPath, meshData);
    meshRead.readFile();
    meshData->setConnectivity();
    MeshMetric meshMetric = MeshMetric(meshData);
    meshMetric.setMetric();

    Solver *solver = new Solver(meshData, inputData);
    SolverTest solverTest = SolverTest(meshData, inputData, solver);
    solverTest.initializeSolver();

    Solution *solution = solver->getSolution();
    string fichierOut = "./tests/solver/Solution_path.dat";
    SolWrite solWriter = SolWrite(fichierOut, meshData, solution);
    solWriter.writeFile();

    delete meshData;
    delete inputData;
    delete solver;
    return;
}