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
    // Connectivité
    _bc2el = _meshData->getBc2el();
    _bc2elStart = _meshData->getBc2elStart();
    _bc2face = _meshData->getBc2face();
    // Métriques
    _face2Normales = _meshData->getFace2Normales();
    return;
}

void Solver::initializeSolution()
{
    //Initialisaton de la solution
    W.rho.assign(meshDim.NELEM + meshDim.NBOUNDARY, 1);
    W.U.assign(meshDim.NELEM + meshDim.NBOUNDARY, props.Ma * sqrt(props.gamma) * cos(props.AOA));
    W.V.assign(meshDim.NELEM + meshDim.NBOUNDARY, props.Ma * sqrt(props.gamma) * sin(props.AOA));
    W.E.assign(meshDim.NELEM + meshDim.NBOUNDARY, 1 / (props.gamma - 1) + 0.5 * props.gamma * props.Ma * props.Ma);
    W.p.assign(meshDim.NELEM + meshDim.NBOUNDARY, 1);

    dW.rho.assign(meshDim.NELEM, 0.);
    dW.U.assign(meshDim.NELEM, 0.);
    dW.V.assign(meshDim.NELEM, 0.);
    dW.E.assign(meshDim.NELEM, 0.);
    dW.p.assign(meshDim.NELEM, 0.);
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
                    int iCelld = _bc2el->at(2 * iBoundary);
                    int iCellb = _bc2el->at(2 * iBoundary + 1);
                    int iFace = _bc2face->at(iBoundary);
                    double Vn;
                    computeVn(W, iCelld, iFace, Vn);
                    if (Vn < 0) // inflow
                    {
                        W.rho[iCellb] = 1.;
                        W.U[iCellb] = props.Ma * sqrt(props.gamma) * cos(props.AOA);
                        W.V[iCellb] = props.Ma * sqrt(props.gamma) * sin(props.AOA);
                        W.p[iCellb] = 1.;
                        computeEnergie(W, iCellb);
                    }
                    else // outflow
                    {
                        W.rho[iCellb] = W.rho[iCelld];
                        W.U[iCellb] = W.U[iCelld];
                        W.V[iCellb] = W.V[iCelld];
                        W.p[iCellb] = W.p[iCelld];
                        computeEnergie(W, iCellb);
                    }
                }
            }
            else // Subsonic
            {
                for (int iBoundary = _bc2elStart->at(iMark); iBoundary < _bc2elStart->at(iMark + 1); iBoundary++)
                {
                    int iCelld = _bc2el->at(2 * iBoundary);
                    int iCellb = _bc2el->at(2 * iBoundary + 1);
                    int iFace = _bc2face->at(iBoundary);
                    double Vn;
                    computeVn(W, iCelld, iFace, Vn);
                    double c0 = sqrt(props.gamma * W.p[iCelld] / W.rho[iCelld]);
                    if (Vn < 0) // inflow
                    {
                        double ua = props.Ma * sqrt(props.gamma) * cos(props.AOA);
                        double va = props.Ma * sqrt(props.gamma) * sin(props.AOA);
                        W.p[iCellb] = 0.5 * (1. + W.p[iCelld] - W.rho[iCelld] * c0 * (_face2Normales->at(2 * iFace) * (ua - W.U[iCelld]) + _face2Normales->at(2 * iFace + 1) * (va - W.V[iCelld])));
                        W.rho[iCellb] = 1. + (W.p[iCellb] - 1.) / (c0 * c0);
                        W.U[iCellb] = ua - _face2Normales->at(2 * iFace) * (1. - W.p[iCellb]) / (W.rho[iCelld] * c0);
                        W.V[iCellb] = va - _face2Normales->at(2 * iFace + 1) * (1. - W.p[iCellb]) / (W.rho[iCelld] * c0);
                        computeEnergie(W, iCellb);
                    }
                    else // outflow
                    {
                        W.p[iCellb] = 1.;
                        W.rho[iCellb] = W.rho[iCelld] + (W.p[iCellb] - W.p[iCelld]) / (c0 * c0);
                        W.U[iCellb] = W.U[iCelld] + _face2Normales->at(2 * iFace) * (W.p[iCellb] - W.p[iCelld]) / (W.rho[iCelld] * c0);
                        W.V[iCellb] = W.V[iCelld] + _face2Normales->at(2 * iFace + 1) * (W.p[iCellb] - W.p[iCelld]) / (W.rho[iCelld] * c0);
                        computeEnergie(W, iCellb);
                    }
                }
            }
        }
        else if (type == 1) // Wall
        {
            for (int iBoundary = _bc2elStart->at(iMark); iBoundary < _bc2elStart->at(iMark + 1); iBoundary++)
            {
                int iCelld = _bc2el->at(2 * iBoundary);
                int iCellb = _bc2el->at(2 * iBoundary + 1);
                int iFace = _bc2face->at(iBoundary);
                double Vn;
                computeVn(W, iCelld, iFace, Vn);
                W.rho[iCellb] = W.rho[iCelld];
                W.U[iCellb] = W.U[iCelld] - 2 * Vn * _face2Normales->at(2 * iFace);
                W.V[iCellb] = W.V[iCelld] - 2 * Vn * _face2Normales->at(2 * iFace + 1);
                W.p[iCellb] = W.p[iCelld];
                computeEnergie(W, iCellb);
            }
        }
    }

    return;
}

void Solver::computeEnergie(Solution &solution, int &index)
{
    solution.E[index] = solution.p[index] / ((props.gamma - 1) * solution.rho[index]) + 0.5 * (solution.U[index] * solution.U[index] + solution.V[index] * solution.V[index]);
    solution.H[index] = solution.E[index] + solution.p[index] / solution.rho[index];
    return;
}

void Solver::computeVn(const Solution &solution, int &iCell, int &iFace, double &Vn)
{
    Vn = solution.U[iCell] * _face2Normales->at(2 * iFace) + solution.V[iCell] * _face2Normales->at(2 * iFace + 1);
    return;
}