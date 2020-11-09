/*
/Fichier solWrite.cpp
/Date: octobre 2020
/Projet: Projet Intégrateur 4
*/

/*

#include "./solWrite.hpp"


SolWrite::SolWrite(string &format, shared_ptr<MeshData> meshData)
{
    _format = format;
    _meshData = meshData;
    cout << _format << endl;
    return;
};

SolWrite::~SolWrite()
{
    return;
};

void SolWrite::writeFile()
{
    string vtufile = ".vtu";
    string tecfile = ".dat";
    if ((vtufile) == _format) // pas sure //
{
    //cout << "Face" << endl; follow2
    VtuWriter _vtuWriter = VtuWriter(_format, _meshData);
    _vtuWriter.writeFile();
}
else
{
    printf("Le format de solution n'est pas reconnu (%s).\n", _format.c_str());
    exit(1);
};
return;
}
;



    // else if ((tecfile) == _format)
    // {
    // // TecWriter _tecWriter = TecWriter(_format, _meshData);
    // //_tecWriter.writeFile();
    // }
* /*/