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
    _timeSteps = new vector<double>();
    _W = new Solution();
    _dW = new Solution();
    _F = new Flux();
    _schemes = new Schemes(_meshData, _inputData, _F, _W);
    return;
}

Solver::~Solver()
{
    delete _timeSteps;
    delete _W;
    delete _dW;
    delete _F;
    delete _schemes;
    return;
}

void Solver::initializeSolver()
{
    // Propiétés fluides et paramètres solveur
    _props.Ma = _inputData->getMachNumber();
    _props.AOA = _inputData->getAOA();
    _props.CFL = _inputData->getCFL();
    _props.c = _inputData->getSoundSpeed();
    _props.gamma = _inputData->getRatioCpCv();
    // Dimensions du maillage
    _meshDim.NELEM = _meshData->getNELEM();
    _meshDim.NPOIN = _meshData->getNPOIN();
    _meshDim.NFACE = _meshData->getNFACE();
    _meshDim.NBOUNDARY = _meshData->getNBOUNDARY();
    _meshDim.NMARK = _meshData->getNMARK();
    // Conditions limites
    _conditionsLimites = _inputData->getConditionsLimites();
    // Connectivité
    _bc2el = _meshData->getBc2el();
    _bc2elStart = _meshData->getBc2elStart();
    _bc2face = _meshData->getBc2face();
    // Métriques
    _element2Volumes = _meshData->getElement2Volumes();
    _face2Normales = _meshData->getFace2Normales();
    _CVprojections = _meshData->getCVprojections();
    return;
}

void Solver::initializeSolution()
{
    //Initialisaton de la solution
    _W->rho.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, 1);
    _W->U.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, _props.Ma * sqrt(_props.gamma) * cos(_props.AOA));
    _W->V.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, _props.Ma * sqrt(_props.gamma) * sin(_props.AOA));
    _W->E.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, 1 / (_props.gamma - 1) + 0.5 * _props.gamma * _props.Ma * _props.Ma);
    _W->p.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, 1);
    _W->H.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, 1 / (_props.gamma - 1) + 0.5 * _props.gamma * _props.Ma * _props.Ma + 1);

    _timeSteps->assign(_meshDim.NELEM, 0.);

    _F->rhoV.assign(_meshDim.NFACE, 0);
    _F->rhouV.assign(_meshDim.NFACE, 0);
    _F->rhovV.assign(_meshDim.NFACE, 0);
    _F->rhoHV.assign(_meshDim.NFACE, 0);

    _dW->rho.assign(_meshDim.NELEM, 0.);
    _dW->U.assign(_meshDim.NELEM, 0.);
    _dW->V.assign(_meshDim.NELEM, 0.);
    _dW->E.assign(_meshDim.NELEM, 0.);
    _dW->p.assign(_meshDim.NELEM, 0.);

    return;
}

void Solver::updateBoundaryCells()
{
    for (int iMark = 0; iMark < _meshDim.NMARK; iMark++)
    {
        int type = _conditionsLimites[iMark];
        if (type == 0) // FarField
        {

            if (_props.Ma > 1) // Supersonic
            {
                for (int iBoundary = _bc2elStart->at(iMark); iBoundary < _bc2elStart->at(iMark + 1); iBoundary++)
                {
                    int iCelld = _bc2el->at(2 * iBoundary);
                    int iCellb = _bc2el->at(2 * iBoundary + 1);
                    int iFace = _bc2face->at(iBoundary);
                    double Vn;
                    computeVn(_W, iCelld, iFace, Vn);
                    if (Vn < 0) // inflow
                    {
                        _W->rho[iCellb] = 1.;
                        _W->U[iCellb] = _props.Ma * sqrt(_props.gamma) * cos(_props.AOA);
                        _W->V[iCellb] = _props.Ma * sqrt(_props.gamma) * sin(_props.AOA);
                        _W->p[iCellb] = 1.;
                        computeEnergie(_W, iCellb);
                    }
                    else // outflow
                    {
                        _W->rho[iCellb] = _W->rho[iCelld];
                        _W->U[iCellb] = _W->U[iCelld];
                        _W->V[iCellb] = _W->V[iCelld];
                        _W->p[iCellb] = _W->p[iCelld];
                        computeEnergie(_W, iCellb);
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
                    computeVn(_W, iCelld, iFace, Vn);
                    double c0 = sqrt(_props.gamma * _W->p[iCelld] / _W->rho[iCelld]);
                    if (Vn < 0) // inflow
                    {
                        double ua = _props.Ma * sqrt(_props.gamma) * cos(_props.AOA);
                        double va = _props.Ma * sqrt(_props.gamma) * sin(_props.AOA);
                        _W->p[iCellb] = 0.5 * (1. + _W->p[iCelld] - _W->rho[iCelld] * c0 * (_face2Normales->at(2 * iFace) * (ua - _W->U[iCelld]) + _face2Normales->at(2 * iFace + 1) * (va - _W->V[iCelld])));
                        _W->rho[iCellb] = 1. + (_W->p[iCellb] - 1.) / (c0 * c0);
                        _W->U[iCellb] = ua - _face2Normales->at(2 * iFace) * (1. - _W->p[iCellb]) / (_W->rho[iCelld] * c0);
                        _W->V[iCellb] = va - _face2Normales->at(2 * iFace + 1) * (1. - _W->p[iCellb]) / (_W->rho[iCelld] * c0);
                        computeEnergie(_W, iCellb);
                    }
                    else // outflow
                    {
                        _W->p[iCellb] = 1.;
                        _W->rho[iCellb] = _W->rho[iCelld] + (_W->p[iCellb] - _W->p[iCelld]) / (c0 * c0);
                        _W->U[iCellb] = _W->U[iCelld] + _face2Normales->at(2 * iFace) * (_W->p[iCellb] - _W->p[iCelld]) / (_W->rho[iCelld] * c0);
                        _W->V[iCellb] = _W->V[iCelld] + _face2Normales->at(2 * iFace + 1) * (_W->p[iCellb] - _W->p[iCelld]) / (_W->rho[iCelld] * c0);
                        computeEnergie(_W, iCellb);
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
                computeVn(_W, iCelld, iFace, Vn);
                _W->rho[iCellb] = _W->rho[iCelld];
                _W->U[iCellb] = _W->U[iCelld] - 2 * Vn * _face2Normales->at(2 * iFace);
                _W->V[iCellb] = _W->V[iCelld] - 2 * Vn * _face2Normales->at(2 * iFace + 1);
                _W->p[iCellb] = _W->p[iCelld];
                computeEnergie(_W, iCellb);
            }
        }
    }

    return;
}

void Solver::computeTimeSteps()
{
    double convSpecRadii;
    for (int iElem = 0; iElem < _meshDim.NELEM; iElem++)
    {
        convSpecRadii = (abs(_W->U[iElem]) + _props.c) * _CVprojections->at(2 * iElem) + (abs(_W->V[iElem]) + _props.c) * _CVprojections->at(2 * iElem + 1);
        _timeSteps->at(iElem) = _props.CFL * _element2Volumes->at(iElem) / convSpecRadii;
    }

    return;
}

void Solver::computeEnergie(Solution *solution, int &index)
{
    solution->E[index] = solution->p[index] / ((_props.gamma - 1) * solution->rho[index]) + 0.5 * (solution->U[index] * solution->U[index] + solution->V[index] * solution->V[index]);
    solution->H[index] = solution->E[index] + solution->p[index] / solution->rho[index];
    return;
}

void Solver::computeVn(const Solution *solution, int &iCell, int &iFace, double &Vn)
{
    Vn = solution->U[iCell] * _face2Normales->at(2 * iFace) + solution->V[iCell] * _face2Normales->at(2 * iFace + 1);
    return;
}