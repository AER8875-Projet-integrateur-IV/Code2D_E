/* 
/Fichier meshRead.cpp
/Date: 12 octobre 2020 - 19h36 
/Projet: Projet Intégrateur 4
*/

#include "./meshConnectivity.hpp"

MeshRead::MeshRead(string &path, shared_ptr<MeshData> meshData)
{
    _path = path;
    _meshData = meshData;
    return;
};
MeshRead::~MeshRead()
{
    return;
};
void MeshRead::readFile()
{
    if (ends_With(_path, ".su2"))
    {
        Su2Reader _su2Reader = Su2Reader(_path, _meshData);
        _su2Reader.readFile();
    }
    else
    {
        printf("Le format de maillage n'est pas reconnu (%s).\n", _path.c_str());
        exit(1);
    };
    return;
};