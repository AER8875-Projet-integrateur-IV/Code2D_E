/*
/Fichier solWrite.cpp
/Date: octobre 2020
/Projet: Projet Intégrateur 4
*/

#include "./solWrite.hpp"

SolWrite::SolWrite(string &path, MeshData *meshData, InputData *inputData, Solver *solver)
{
    _path = path;
    _meshData = meshData;
    _solver = solver;
    _inputData = inputData;
    cout << _path << endl;
    _Cp = new vector<double>();
    return;
};

SolWrite::~SolWrite()
{
    delete _Cp;
    return;
};

void SolWrite::writeFile()
{
    writeErrors();
    if (ends_With(_path, ".vtu")) // pas sure //
    {
        //cout << "Face" << endl; follow2
        VtuWriter _vtuWriter = VtuWriter(_path, _meshData);
        _vtuWriter.writeFile();
    }
    else if (ends_With(_path, ".dat"))
    {
        //cout << "Pile" << endl; follow3
        TecWriter _tecWriter = TecWriter(_path, _meshData, _solver);
        _tecWriter.writeFile();
    }
    else
    {
        printf("Le format de solution n'est pas reconnu (%s).\n", _path.c_str());
        exit(1);
    };
    return;
};

void SolWrite::writeErrors()
{
    computeAerodynamicCoefs();
    FILE *fError = fopen("Erreurs.out", "w");
    FILE *fAero = fopen("Aerodynamique.out", "w");
    if ((fError == NULL) | (fAero == NULL))
    {
        printf("Imposible de créer le fichier Erreurs.out ou Aerodynamique.out\n");
        exit(1);
    }
    fprintf(fError, "Nombre d'iteration: %6d\n", _inputData->getIterationMax());
    fprintf(fError, "Itérations\tError rho\t\tError rhoU\t\tError rhoV\t\tError rhoE\n");
    vector<double> *errors = _solver->getErrors();
    for (int iter = 0; iter < _inputData->getIterationMax(); iter++)
    {
        fprintf(fError, "%-6d\t\t%-8.3e\t\t%-8.3e\t\t%-8.3e\t\t%-8.3e\n", iter + 1, errors->at(4 * iter + 0), errors->at(4 * iter + 1), errors->at(4 * iter + 2), errors->at(4 * iter + 3));
    }
    fprintf(fAero, "Taille Maillage: %9.6e\n", _solver->getMeshSize());
    fprintf(fAero, "Coefficients Cl, CD: %9.4e\t%9.4e\n", _Cl, _Cd);
    fprintf(fAero, "Coefficients de presions\nx/c\t\t\tCp\n");
    for (size_t i = 0; i < _Cp->size() / 2; i++)
    {
        fprintf(fAero, "%-9.4e\t%-9.4e\n", _Cp->at(2 * i), _Cp->at(2 * i + 1));
    }

    fclose(fError);
    fclose(fAero);
    return;
}

void SolWrite::computeAerodynamicCoefs()
{
    int mark = 0;
    Solution *W = _solver->getSolution();
    double q_inf = 0.5 * 1. * pow(_inputData->getMachNumber(), 2);
    for (int iMark = 0; iMark < _meshData->getNMARK(); iMark++)
    {
        int type = _inputData->getConditionsLimites()[iMark];
        if (type == 1) // Wall
        {
            mark = iMark;
            break;
        }
    }
    double cn = 0;
    double ca = 0;
    double cp;
    _Cp->reserve(2 * (_meshData->getBc2elStart()->at(mark + 1) - _meshData->getBc2elStart()->at(mark)));
    for (int iBoundary = _meshData->getBc2elStart()->at(mark); iBoundary < _meshData->getBc2elStart()->at(mark + 1); iBoundary++)
    {
        int iFace = _meshData->getBc2face()->at(iBoundary);
        int iCelld = _meshData->getBc2el()->at(2 * iBoundary);
        cp = (W->p[iCelld] - 1.) / q_inf;
        _Cp->push_back(_meshData->getFace2Centres()->at(2 * iFace + 0));
        _Cp->push_back(cp);
        cn += cp * _meshData->getFace2Normales()->at(2 * iFace + 1) * _meshData->getFace2Aires()->at(iFace);
        ca += cp * _meshData->getFace2Normales()->at(2 * iFace + 0) * _meshData->getFace2Aires()->at(iFace);
        ;
    }
    cout << "Cn = " << cn << " Ca = " << ca << endl;
    _Cl = cn * cos(_inputData->getAOA()) - ca * sin(_inputData->getAOA());
    _Cd = cn * sin(_inputData->getAOA()) + ca * cos(_inputData->getAOA());
    return;
}