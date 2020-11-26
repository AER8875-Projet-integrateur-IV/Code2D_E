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
    return;
};

SolWrite::~SolWrite()
{
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
    ofstream fichier("Erreurs.out");
    fichier << scientific << setprecision(3);
    fichier << "Taille maillage: " << _solver->getMeshSize() << "\n"
            << "Coefficient Aerodynamique Cl, Cd:\n"
            << "Nombre d'itération: " << _inputData->getIterationMax() << "\n";
    fichier << "Itérations\t\tError rho\t\tError rhoU\t\tError rhoV\t\tError rhoE\n";
    vector<double> *errors = _solver->getErrors();
    for (int iter = 0; iter < _inputData->getIterationMax(); iter++)
    {
        fichier << iter + 1
                << "\t\t\t\t" << errors->at(4 * iter + 0)
                << "\t\t" << errors->at(4 * iter + 1)
                << "\t\t" << errors->at(4 * iter + 2)
                << "\t\t" << errors->at(4 * iter + 3)
                << "\n";
    }

    fichier.close();
    return;
}