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
    _errors = new vector<double>();
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
    delete _errors;
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
    // Dimensions du maillage
    _meshDim.NELEM = _meshData->getNELEM();
    _meshDim.NPOIN = _meshData->getNPOIN();
    _meshDim.NDIME = _meshData->getNDIME();
    _meshDim.NFACE = _meshData->getNFACE();
    _meshDim.NBOUNDARY = _meshData->getNBOUNDARY();
    _meshDim.NMARK = _meshData->getNMARK();
    _meshDim.meshSize = 0;
    for (int iElem = 0; iElem < _meshDim.NELEM; iElem++)
    {
        _meshDim.meshSize += _element2Volumes->at(iElem);
    }
    _meshDim.meshSize = std::pow(_meshDim.meshSize / _meshDim.NELEM, 1. / _meshDim.NDIME);
    // Conditions limites
    _conditionsLimites = _inputData->getConditionsLimites();
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
    _W->rhoE.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, 1. / (_props.gamma - 1) + 0.5 * _props.gamma * _props.Ma * _props.Ma);
    _W->p.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, 1.);
    _W->H.assign(_meshDim.NELEM + _meshDim.NBOUNDARY, 1. / (_props.gamma - 1) + 0.5 * _props.gamma * _props.Ma * _props.Ma + 1.);

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

    _errors->reserve(4 * _props.Niter);

    updateBoundaryCells();
    cout << "\tFin de la fonction initializeSolution() de la classe Solver.\n";
    return;
}

void Solver::initializeEachIteration()
{
    fill(_R->rhoVds.begin(), _R->rhoVds.end(), 0.);
    fill(_R->rhouVds.begin(), _R->rhouVds.end(), 0.);
    fill(_R->rhovVds.begin(), _R->rhovVds.end(), 0.);
    fill(_R->rhoHVds.begin(), _R->rhoHVds.end(), 0.);
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
                    double Vn; // = _props.Ma * sqrt(_props.gamma) * cos(_props.AOA) * _face2Normales->at(2 * iFace + 0) + _props.Ma * sqrt(_props.gamma) * sin(_props.AOA) * _face2Normales->at(2 * iFace + 1);
                    computeVn(_W, iCelld, iFace, Vn);
                    if (Vn < 0.) // inflow
                    {
                        _W->rho[iCellb] = 1.;
                        _W->rhoU[iCellb] = _props.Ma * sqrt(_props.gamma) * cos(_props.AOA);
                        _W->rhoV[iCellb] = _props.Ma * sqrt(_props.gamma) * sin(_props.AOA);
                        _W->p[iCellb] = 1.;
                        computeEnergie(_W, iCellb);
                    }
                    else // outflow
                    {
                        _W->rho[iCellb] = _W->rho[iCelld];
                        _W->rhoU[iCellb] = _W->rhoU[iCelld];
                        _W->rhoV[iCellb] = _W->rhoV[iCelld];
                        _W->rhoE[iCellb] = _W->rhoE[iCelld];
                        _W->p[iCellb] = (_props.gamma - 1.) * (_W->rhoE[iCellb] - 0.5 * (_W->rhoU[iCellb] * _W->rhoU[iCellb] + _W->rhoV[iCellb] * _W->rhoV[iCellb]) / _W->rho[iCellb]);
                        _W->H[iCellb] = (_W->rhoE[iCellb] + _W->p[iCellb]) / _W->rho[iCellb];
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
                    if (Vn < 0.) // inflow
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
                double Vn;
                computeVn(_W, iCelld, iFace, Vn);
                _W->rho[iCellb] = _W->rho[iCelld];
                _W->rhoU[iCellb] = (_W->rhoU[iCelld] / _W->rho[iCelld] - 2 * Vn * _face2Normales->at(2 * iFace)) * _W->rho[iCellb];
                _W->rhoV[iCellb] = (_W->rhoV[iCelld] / _W->rho[iCelld] - 2 * Vn * _face2Normales->at(2 * iFace + 1)) * _W->rho[iCellb];
                _W->p[iCellb] = _W->p[iCelld];
                computeEnergie(_W, iCellb);
            }
        }
    }

    return;
}

void Solver::computeTimeSteps()
{
    for (int iElem = 0; iElem < _meshDim.NELEM; iElem++)
    {
        double convSpecRadii;
        double c;
        c = sqrt(_props.gamma * _W->p[iElem] / _W->rho[iElem]);
        convSpecRadii = (abs(_W->rhoU[iElem] / _W->rho[iElem]) + c) * _CVprojections->at(2 * iElem + 0) + (abs(_W->rhoV[iElem] / _W->rho[iElem]) + c) * _CVprojections->at(2 * iElem + 1);
        _timeSteps->at(iElem) = _props.CFL * _element2Volumes->at(iElem) / convSpecRadii;
    }

    return;
}

void Solver::computeResiduals()
{

    for (int iFace = 0; iFace < _meshDim.NFACE - _meshDim.NBOUNDARY; iFace++)
    {
        int L, R;
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
        int L, R;
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
    /*     for (int iMark = 0; iMark < _meshDim.NMARK; iMark++)
    {
        cout << _conditionsLimites[iMark] << endl;
        ;
        for (int iBoundary = _bc2elStart->at(iMark); iBoundary < _bc2elStart->at(iMark + 1); iBoundary++)
        {
            int iFace = _bc2face->at(iBoundary);
            cout << "Flux: " << _F->rhoV[iFace] << "\t\t" << _F->rhouV[iFace] << "\t\t" << _F->rhovV[iFace] << "\t\t" << _F->rhoHV[iFace] << "\n";
        }
    } */

    computeResiduals();
    _schemes->computeConservativesVariables();
    updateW();
    updateBoundaryCells();
    computeErrors();
    return;
}

void Solver::runSolver()
{
    cout << "Démarrage de l'éxécution du solveur\n";
    initializeSolution();
    /*     cout << "Afichage de la solution initiale: \n";
    for (int iElem = 0; iElem < _meshDim.NELEM; iElem++)
    {
        cout << "Element " << iElem << " "
             << _W->rho[iElem] << " "
             << _W->rhoU[iElem] << " "
             << _W->rhoV[iElem] << " "
             << _W->rhoE[iElem] << " "
             << _W->p[iElem] << " "
             << _W->H[iElem] << "\n";
    } */
    cout << "Itérations\tError rho\tError rhoU\tError rhoV\tError rhoE\n";
    for (int iter = 0; iter < _props.Niter; iter++)
    {
        makeOneIteration();
        cout << iter + 1
             << "\t" << _errors->at(4 * iter + 0)
             << "\t" << _errors->at(4 * iter + 1)
             << "\t" << _errors->at(4 * iter + 2)
             << "\t" << _errors->at(4 * iter + 3)
             << "\n";
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

void Solver::computeErrors()
{
    vector<double> error = {0., 0., 0., 0.};
    for (int iElem = 0; iElem < _meshDim.NELEM; iElem++)
    {
        error[0] += pow(_dW->rho[iElem], 2.);
        error[1] += pow(_dW->rhoU[iElem], 2.);
        error[2] += pow(_dW->rhoV[iElem], 2.);
        error[3] += pow(_dW->rhoE[iElem], 2.);
    }
    _errors->push_back(sqrt(error[0] / _meshDim.NELEM));
    _errors->push_back(sqrt(error[1] / _meshDim.NELEM));
    _errors->push_back(sqrt(error[2] / _meshDim.NELEM));
    _errors->push_back(sqrt(error[3] / _meshDim.NELEM));
}

void Solver::computeEnergie(Solution *solution, int &index)
{
    solution->rhoE[index] = solution->p[index] / (_props.gamma - 1.) + 0.5 * (solution->rhoU[index] * solution->rhoU[index] + solution->rhoV[index] * solution->rhoV[index]) / solution->rho[index];
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

Residual *Solver::getResidus() const
{
    return _R;
}

vector<double> *Solver::getErrors() const
{
    return _errors;
}

double Solver::getMeshSize() const
{
    return _meshDim.meshSize;
}