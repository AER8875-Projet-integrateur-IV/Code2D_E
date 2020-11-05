/* 
/Fichier solver.cpp
/Date: 05 novembre 2020 - 02h39
/Projet: Projet Intégrateur 4
*/

#include "./solver.hpp"

Solver::Solver(shared_ptr<MeshData> meshData, shared_ptr<InputData> inputData)
{
    _meshData = meshData;
    _inputData = inputData;
    return;
}

Solver::~Solver()
{
    return;
}

void Solver::initializeSolver()
{
    props.Ma = _inputData->getMachNumber();
    props.AOA = _inputData->getAOA();
    props.CFL = _inputData->getCFL();
    props.c = _inputData->getSoundSpeed();
    props.gamma = _inputData->getRatioCpCv();
    return;
}

void Solver::initializeSolution()
{
    return;
}