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
    _R = new Residual();
    _schemes = new Schemes(_meshData, _inputData, _F, _W, _R, _dW, _timeSteps);
    return;
}

Solver::~Solver()
{
    delete _timeSteps;
    delete _W;
    delete _dW;
    delete _F;
    delete _R;
    delete _schemes;
    return;
}

void Solver::initializeSolver()
{
    cout << "Début de la fonction initializeSolver() de la classe Solver.\n";
    // Propiétés fluides et paramètres solveur
    _props.Ma = _inputData->getMachNumber();
    _props.AOA = _inputData->getAOA();
    _props.CFL = _inputData->getCFL();
    _props.gamma = _inputData->getRatioCpCv();
    _props.c = sqrt(_props.gamma); //_inputData->getSoundSpeed();
    _props.Niter = _inputData->getIterationMax();
    // Dimensions du maillage
    _meshDim.NELEM = _meshData->getNELEM();
    _meshDim.NPOIN = _meshData->getNPOIN();
    _meshDim.NFACE = _meshData->getNFACE();
    _meshDim.NBOUNDARY = _meshData->getNBOUNDARY();
    _meshDim.NMARK = _meshData->getNMARK();
    // Conditions limites
    _conditionsLimites = _inputData->getConditionsLimites();
    // Connectivité
    _esuf = _meshData->getEsuf();
    _bc2el = _meshData->getBc2el();
    _bc2elStart = _meshData->getBc2elStart();
    _bc2face = _meshData->getBc2face();
    // Métriques
    _element2Volumes = _meshData->getElement2Volumes();
    _face2Aires = _meshData->getFace2Aires();
    _face2Normales = _meshData->getFace2Normales();
    _CVprojections = _meshData->getCVprojections();
    cout << "\tFin de la fonction initializeSolver() de la classe Solver.\n";
    return;
}

void Solver::initializeSolution()
{
    cout << "Début de la fonction initializeSolution() de la classe Solver.\n";
    //Initialisaton de la solution
    _W->rho.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, 1.);
    _W->rhoU.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, _props.Ma * sqrt(_props.gamma) * cos(_props.AOA));
    _W->rhoV.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, _props.Ma * sqrt(_props.gamma) * sin(_props.AOA));
    _W->rhoE.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, 1 / (_props.gamma - 1) + 0.5 * _props.gamma * _props.Ma * _props.Ma);
    _W->p.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, 1.);
    _W->H.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, 1 / (_props.gamma - 1) + 0.5 * _props.gamma * _props.Ma * _props.Ma + 1.);

    _timeSteps->assign(_meshDim.NELEM, 0.);

    _F->rhoV.assign(_meshDim.NFACE, 0.);
    _F->rhouV.assign(_meshDim.NFACE, 0.);
    _F->rhovV.assign(_meshDim.NFACE, 0.);
    _F->rhoHV.assign(_meshDim.NFACE, 0.);

    _R->rhoVds.assign(_meshDim.NELEM, 0.);
    _R->rhouVds.assign(_meshDim.NELEM, 0.);
    _R->rhovVds.assign(_meshDim.NELEM, 0.);
    _R->rhoHVds.assign(_meshDim.NELEM, 0.);

    _dW->rho.assign(_meshDim.NELEM, 0.);
    _dW->rhoU.assign(_meshDim.NELEM, 0.);
    _dW->rhoV.assign(_meshDim.NELEM, 0.);
    _dW->rhoE.assign(_meshDim.NELEM, 0.);

    updateBoundaryCells();
    cout << "\tFin de la fonction initializeSolution() de la classe Solver.\n";
    return;
}

void Solver::initializeEachIteration()
{
    //fill(_timeSteps->begin(), _timeSteps->end(), 0.);

    /*     fill(_F->rhoV.begin(), _F->rhoV.end(), 0.);
    fill(_F->rhouV.begin(), _F->rhouV.end(), 0.);
    fill(_F->rhovV.begin(), _F->rhovV.end(), 0.);
    fill(_F->rhoHV.begin(), _F->rhoHV.end(), 0.);
 */
    fill(_R->rhoVds.begin(), _R->rhoVds.end(), 0.);
    fill(_R->rhouVds.begin(), _R->rhouVds.end(), 0.);
    fill(_R->rhovVds.begin(), _R->rhovVds.end(), 0.);
    fill(_R->rhoHVds.begin(), _R->rhoHVds.end(), 0.);

    /*     fill(_dW->rho.begin(), _dW->rho.end(), 0.);
    fill(_dW->rhoU.begin(), _dW->rhoU.end(), 0.);
    fill(_dW->rhoV.begin(), _dW->rhoV.end(), 0.);//
    fill(_dW->rhoE.begin(), _dW->rhoE.end(), 0.); */
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
                    //int iCellb = _bc2el->at(2 * iBoundary + 1);
                    int iFace = _bc2face->at(iBoundary);
                    double Vn = _props.Ma * sqrt(_props.gamma) * cos(_props.AOA) * _face2Normales->at(2 * iFace + 0) + _props.Ma * sqrt(_props.gamma) * sin(_props.AOA) * _face2Normales->at(2 * iFace + 1);
                    //computeVn(_W, iCelld, iFace, Vn);
                    if (Vn < 0) // inflow
                    {

                        _F->rhoV[iFace] = 1. * Vn;
                        _F->rhouV[iFace] = 1. * _props.Ma * sqrt(_props.gamma) * cos(_props.AOA) * Vn;
                        _F->rhovV[iFace] = 1. * _props.Ma * sqrt(_props.gamma) * sin(_props.AOA) * Vn;
                        _F->rhoHV[iFace] = 1. * (1 / (_props.gamma - 1) + 0.5 * _props.gamma * _props.Ma * _props.Ma + 1.) * Vn;
                        //computeEnergie(_W, iCellb);
                    }
                    else // outflow
                    {
                        computeVn(_W, iCelld, iFace, Vn);
                        _F->rhoV[iFace] = _W->rho[iCelld] * Vn;
                        _F->rhouV[iFace] = _W->rhoU[iCelld] * Vn;
                        _F->rhovV[iFace] = _W->rhoV[iCelld] * Vn;
                        _F->rhoHV[iFace] = _W->rho[iCelld] * _W->H[iCelld] * Vn;
                        //computeEnergie(_W, iCellb);
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
                        _W->p[iCellb] = 0.5 * (1. + _W->p[iCelld] - _W->rho[iCelld] * c0 * (_face2Normales->at(2 * iFace) * (ua - _W->rhoU[iCelld] / _W->rho[iCelld]) + _face2Normales->at(2 * iFace + 1) * (va - _W->rhoV[iCelld] / _W->rho[iCelld])));
                        _W->rho[iCellb] = 1. + (_W->p[iCellb] - 1.) / (c0 * c0);
                        _W->rhoU[iCellb] = _W->rho[iCellb] * (ua - _face2Normales->at(2 * iFace) * (1. - _W->p[iCellb]) / (_W->rho[iCelld] * c0));
                        _W->rhoV[iCellb] = _W->rho[iCellb] * (va - _face2Normales->at(2 * iFace + 1) * (1. - _W->p[iCellb]) / (_W->rho[iCelld] * c0));
                        computeEnergie(_W, iCellb);
                    }
                    else // outflow
                    {
                        _W->p[iCellb] = 1.;
                        _W->rho[iCellb] = _W->rho[iCelld] + (_W->p[iCellb] - _W->p[iCelld]) / (c0 * c0);
                        _W->rhoU[iCellb] = _W->rho[iCellb] * (_W->rhoU[iCelld] / _W->rho[iCelld] + _face2Normales->at(2 * iFace) * (_W->p[iCellb] - _W->p[iCelld]) / (_W->rho[iCelld] * c0));
                        _W->rhoV[iCellb] = _W->rho[iCellb] * (_W->rhoV[iCelld] / _W->rho[iCelld] + _face2Normales->at(2 * iFace + 1) * (_W->p[iCellb] - _W->p[iCelld]) / (_W->rho[iCelld] * c0));
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
                //double Vn;
                //computeVn(_W, iCelld, iFace, Vn);
                _F->rhoV[iCellb] = 0;                                                  //_W->rho[iCelld];
                _F->rhouV[iCellb] = _face2Normales->at(2 * iFace) * _W->p[iCelld];     //_W->rhoU[iCelld] - 2 * Vn * _face2Normales->at(2 * iFace) * _W->rho[iCellb];
                _F->rhovV[iCellb] = _face2Normales->at(2 * iFace + 1) * _W->p[iCelld]; //_W->rhoV[iCelld] - 2 * Vn * _face2Normales->at(2 * iFace + 1) * _W->rho[iCellb];
                _F->rhoHV[iCellb] = 0;                                                 //_W->p[iCelld];
                //computeEnergie(_W, iCellb);
            }
        }
    }

    return;
}

void Solver::computeTimeSteps()
{
    double convSpecRadii;
    double c;
    for (int iElem = 0; iElem < _meshDim.NELEM; iElem++)
    {
        c = sqrt(_props.gamma * _W->p[iElem] / _W->rho[iElem]);
        convSpecRadii = (abs(_W->rhoU[iElem] / _W->rho[iElem]) + c) * _CVprojections->at(2 * iElem + 0) + (abs(_W->rhoV[iElem] / _W->rho[iElem]) + c) * _CVprojections->at(2 * iElem + 1);
        _timeSteps->at(iElem) = _props.CFL * _element2Volumes->at(iElem) / convSpecRadii;
    }

    return;
}

void Solver::computeResiduals()
{

    int L, R;
    for (int iFace = 0; iFace < _meshDim.NFACE - _meshDim.NBOUNDARY; iFace++)
    {
        L = _esuf->at(2 * iFace + 0);
        R = _esuf->at(2 * iFace + 1);
        _R->rhoVds[R] -= _F->rhoV[iFace] * _face2Aires->at(iFace);
        _R->rhoVds[L] += _F->rhoV[iFace] * _face2Aires->at(iFace);

        _R->rhouVds[R] -= _F->rhouV[iFace] * _face2Aires->at(iFace);
        _R->rhouVds[L] += _F->rhouV[iFace] * _face2Aires->at(iFace);

        _R->rhovVds[R] -= _F->rhovV[iFace] * _face2Aires->at(iFace);
        _R->rhovVds[L] += _F->rhovV[iFace] * _face2Aires->at(iFace);

        _R->rhoHVds[R] -= _F->rhoHV[iFace] * _face2Aires->at(iFace);
        _R->rhoHVds[L] += _F->rhoHV[iFace] * _face2Aires->at(iFace);
    }
    for (int iFace = _meshDim.NFACE - _meshDim.NBOUNDARY; iFace < _meshDim.NFACE; iFace++)
    {
        L = _esuf->at(2 * iFace + 0);
        _R->rhoVds[L] += _F->rhoV[iFace] * _face2Aires->at(iFace);

        _R->rhouVds[L] += _F->rhouV[iFace] * _face2Aires->at(iFace);

        _R->rhovVds[L] += _F->rhovV[iFace] * _face2Aires->at(iFace);

        _R->rhoHVds[L] += _F->rhoHV[iFace] * _face2Aires->at(iFace);
    }

    return;
}

void Solver::updateW()
{
    for (int iElem = 0; iElem < _meshDim.NELEM; iElem++)
    {
        _W->rho[iElem] += _dW->rho[iElem];
        _W->rhoU[iElem] += _dW->rhoU[iElem];
        _W->rhoV[iElem] += _dW->rhoV[iElem];
        _W->rhoE[iElem] += _dW->rhoE[iElem];
        _W->p[iElem] = (_props.gamma - 1) * (_W->rhoE[iElem] - (_W->rhoU[iElem] * _W->rhoU[iElem] + _W->rhoV[iElem] * _W->rhoV[iElem]) / (2 * _W->rho[iElem]));
        _W->H[iElem] = (_W->rhoE[iElem] + _W->p[iElem]) / _W->rho[iElem];
    }
    return;
}

void Solver::makeOneIteration()
{
    initializeEachIteration();
    computeTimeSteps();
    _schemes->computeConvectivesFlux();
    computeResiduals();
    _schemes->computeConservativesVariables();
    updateW();
    updateBoundaryCells();
    for (int iElem = 0; iElem < _meshDim.NELEM; iElem++)
    {
        cout << "Element " << iElem << " "
             << _W->rho[iElem] << " "
             << _W->rhoU[iElem] << " "
             << _W->rhoV[iElem] << " "
             << _W->rhoE[iElem] << " "
             << _W->p[iElem] << " "
             << _W->H[iElem] << " "
             << "dT " << _timeSteps->at(iElem) << " "
             << "R "
             << _R->rhoVds[iElem] << " "
             << _R->rhouVds[iElem] << " "
             << _R->rhovVds[iElem] << " "
             << _R->rhoHVds[iElem] << " "
             << "dW: "
             << _dW->rho[iElem] << " "
             << _dW->rhoU[iElem] << " "
             << _dW->rhoV[iElem] << " "
             << _dW->rhoE[iElem] << "\n";
    }
    return;
}

void Solver::runSolver()
{
    cout << "Démarrage de l'éxécution du solveur\n";
    initializeSolution();
    cout << "Afichage de la solution initiale: \n";
    for (int iElem = 0; iElem < _meshDim.NELEM; iElem++)
    {
        cout << "Element " << iElem << " "
             << _W->rho[iElem] << " "
             << _W->rhoU[iElem] << " "
             << _W->rhoV[iElem] << " "
             << _W->rhoE[iElem] << " "
             << _W->p[iElem] << " "
             << _W->H[iElem] << "\n";
    }
    for (int iter = 0; iter < _props.Niter; iter++)
    {
        makeOneIteration();
    }

    cout << "Fin de l'éxécution du solveur\n";
    cout << "Afichage de la solution Finale: \n";
    for (int iElem = 0; iElem < _meshDim.NELEM; iElem++)
    {
        cout << "Element " << iElem << " "
             << _W->rho[iElem] << " "
             << _W->rhoU[iElem] << " "
             << _W->rhoV[iElem] << " "
             << _W->rhoE[iElem] << " "
             << _W->p[iElem] << " "
             << _W->H[iElem] << "\n";
    }
    return;
}

void Solver::computeEnergie(Solution *solution, int &index)
{
    solution->rhoE[index] = solution->p[index] / (_props.gamma - 1) + 0.5 * (solution->rhoU[index] * solution->rhoU[index] + solution->rhoV[index] * solution->rhoV[index]) / solution->rho[index];
    solution->H[index] = (solution->rhoE[index] + solution->p[index]) / solution->rho[index];
    return;
}

void Solver::computeVn(const Solution *solution, int &iCell, int &iFace, double &Vn)
{
    Vn = (solution->rhoU[iCell] * _face2Normales->at(2 * iFace) + solution->rhoV[iCell] * _face2Normales->at(2 * iFace + 1)) / solution->rho[iCell];
    return;
}

// Getters

Solution *Solver::getSolution() const
{
    return _W;
}