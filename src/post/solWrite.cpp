/*
/Fichier solWrite.cpp
/Date: octobre 2020
/Projet: Projet Intégrateur 4
*/

#include "./solWrite.hpp"

SolWrite::SolWrite(string &path, MeshData *meshData, Solution *solution)
{
    _path = path;
    _meshData = meshData;
    _solution = solution;
    cout << _path << endl;
    return;
};

SolWrite::~SolWrite()
{
    return;
};

void SolWrite::writeFile()
{
    if (ends_With(_path, ".vtu")) // pas sure //
    {
        //cout << "Face" << endl; follow2
        VtuWriter _vtuWriter = VtuWriter(_path, _meshData);
        _vtuWriter.writeFile();
    }
    else if (ends_With(_path, ".dat"))
    {
        //cout << "Pile" << endl; follow3
        TecWriter _tecWriter = TecWriter(_path, _meshData, _solution);
        _tecWriter.writeFile();
    }
    else
    {
        printf("Le format de solution n'est pas reconnu (%s).\n", _path.c_str());
        exit(1);
    };
    return;
};
