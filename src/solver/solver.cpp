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
    initializeSolver();
    return;
}

Solver::~Solver()
{
    return;
}

void Solver::initializeSolver()
{
    // Propiétés fluides et paramètres solveur
    props.Ma = _inputData->getMachNumber();
    props.AOA = _inputData->getAOA();
    props.CFL = _inputData->getCFL();
    props.c = _inputData->getSoundSpeed();
    props.gamma = _inputData->getRatioCpCv();
    // Dimensions du maillage
    meshDim.NELEM = _meshData->getNELEM();
    meshDim.NPOIN = _meshData->getNPOIN();
    meshDim.NFACE = _meshData->getNFACE();
    meshDim.NBOUNDARY = _meshData->getNBOUNDARY();
    return;
}

void Solver::initializeSolution()
{
    //Initialisaton de la solution
    W.rho.assign(meshDim.NELEM + meshDim.NBOUNDARY, 1);
    W.rhoU.assign(meshDim.NELEM + meshDim.NBOUNDARY, props.Ma * sqrt(props.gamma) * cos(props.AOA));
    W.rhoV.assign(meshDim.NELEM + meshDim.NBOUNDARY, props.Ma * sqrt(props.gamma) * sin(props.AOA));
    W.rhoE.assign(meshDim.NELEM + meshDim.NBOUNDARY, 1 / (props.gamma - 1) + 0.5 * props.gamma * props.Ma * props.Ma);

    dW.rho.assign(meshDim.NELEM, 0.);
    dW.rhoU.assign(meshDim.NELEM, 0.);
    dW.rhoV.assign(meshDim.NELEM, 0.);
    dW.rhoE.assign(meshDim.NELEM, 0.);
    return;
}