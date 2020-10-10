#include "./meshRead.hpp"

MeshRead::MeshRead(string &path, MeshData *meshData)
{
    _path = path;
    _meshData = meshData;
    _su2Reader = Su2Reader(_path, _meshData);
    return;
};
MeshRead::~MeshRead()
{
    return;
};
void MeshRead::readFile()
{
    if (endsWith(_path, ".su2"))
    {
        _su2Reader.readFile();
    }
    else
    {
        printf("Le format de maillage n'est pas reconnu (%s).\n", _path.c_str());
        system("pause");
        exit(1);
    };
    return;
};