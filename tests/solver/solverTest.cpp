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
    string path1 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/naca0012_euler_9x9x1_O_1B.su2";
    string path2 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/square_5x5.su2";
    string path3 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/NACA0012_65_mixed.su2";
    string path4 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/maillage_exemple_prof.su2";
    string path5 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/naca0012_euler_33x33x1_O_1B.su2";
    MeshRead meshRead = MeshRead(path1, meshData);
    meshRead.readFile();
    meshData->setConnectivity();
    MeshMetric meshMetric = MeshMetric(meshData);
    meshMetric.setMetric();
    string pathInput = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/inputData/Example.project";
    InputData *inputData = new InputData(pathInput);
    inputData->readFile();
    Solver *solver = new Solver(meshData, inputData);

    SolverTest solverTest = SolverTest(meshData, inputData, solver);
    solverTest.initializeSolver();
    Solution *solution = solver->getSolution();
    string fichierOut = "./tests/solver/Solution_path_1.dat";
    SolWrite solWriter = SolWrite(fichierOut, meshData, solution);
    solWriter.writeFile();
    delete meshData;
    delete inputData;
    delete solver;
    return;
}