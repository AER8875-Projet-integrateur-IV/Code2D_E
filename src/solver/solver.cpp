/* 
/Fichier solver.cpp
/Date: 05 novembre 2020 - 02h39
/Projet: Projet Intégrateur 4
*/

#include "./solver.hpp"

Solver::Solver(MeshData *meshData, InputData *inputData)
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
    meshDim.NMARK = _meshData->getNMARK();
    // Conditions limites
    _conditionsLimites = _inputData->getConditionsLimites();
    _bc2el = _meshData->getBc2el();
    _bc2elStart = _meshData->getBc2elStart();
    // Connectivité

    // Métriques
    _face2Normales = _meshData->getFace2Normales();
    return;
}

void Solver::initializeSolution()
{
    //Initialisaton de la solution
    W.rho.assign(meshDim.NELEM + meshDim.NBOUNDARY, 1);
    W.rhoU.assign(meshDim.NELEM + meshDim.NBOUNDARY, props.Ma * sqrt(props.gamma) * cos(props.AOA));
    W.rhoV.assign(meshDim.NELEM + meshDim.NBOUNDARY, props.Ma * sqrt(props.gamma) * sin(props.AOA));
    W.rhoE.assign(meshDim.NELEM + meshDim.NBOUNDARY, 1 / (props.gamma - 1) + 0.5 * props.gamma * props.Ma * props.Ma);
    W.Vn.reserve(meshDim.NELEM + meshDim.NBOUNDARY);

    dW.rho.assign(meshDim.NELEM, 0.);
    dW.rhoU.assign(meshDim.NELEM, 0.);
    dW.rhoV.assign(meshDim.NELEM, 0.);
    dW.rhoE.assign(meshDim.NELEM, 0.);
    return;
}

void Solver::updateBoundaryCells()
{
    for (int iMark = 0; iMark < meshDim.NMARK; iMark++)
    {
        int type = _conditionsLimites[iMark];
        if (type == 0) // FarField
        {

            if (props.Ma > 1) // Supersonic
            {
                for (int iBoundary = _bc2elStart->at(iMark); iBoundary < _bc2elStart->at(iMark + 1); iBoundary++)
                {
                    //int iCelld = _bc2el[->at(2 * iBoundary);
                    int iCellb = _bc2el->at(2 * iBoundary + 1);
                    W.rho[iCellb] = 1;
                    W.rhoU[iCellb] = props.Ma * sqrt(props.gamma) * cos(props.AOA);
                    W.rhoV[iCellb] = props.Ma * sqrt(props.gamma) * cos(props.AOA);
                    W.rhoE[iCellb] = 1 / (props.gamma - 1) + 0.5 * props.gamma * props.Ma * props.Ma;
                }
            }
            else
            {
                continue;
            }
        }
        else if (type == 1) // Wall
        {
            for (int iBoundary = _bc2elStart->at(iMark); iBoundary < _bc2elStart->at(iMark + 1); iBoundary++)
            {
                //int iCelld = _bc2el[2 * iBoundary];
                int iCellb = _bc2el->at(2 * iBoundary + 1);
                W.rho[iCellb] = 1;
                W.rhoU[iCellb] = props.Ma * sqrt(props.gamma) * cos(props.AOA);
                W.rhoV[iCellb] = props.Ma * sqrt(props.gamma) * cos(props.AOA);
                W.rhoE[iCellb] = 1 / (props.gamma - 1) + 0.5 * props.gamma * props.Ma * props.Ma;
            }
        }
    }

    return;
}