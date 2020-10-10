#include <su2Reader.hpp>

Su2Reader::Su2Reader(string &path, MeshData *meshData)
{
    _path = path;
    _meshData = meshData;
    _inputFile == NULL;
    return;
}

Su2Reader::~Su2Reader()
{
    if (_inputFile != NULL)
    {
        fclose(_inputFile);
    }
    return;
}
void Su2Reader::readFile()
{
    if (!isFileValid())
    {
        // if there is a probleme when opening the file
        printf("Le programme n'arrive pas à ouvrir le fichier: %s\n", _path.c_str());
        system("pause");
        exit(1);
    }

    return;
}
bool Su2Reader::isFileValid()
{
    if (_inputFile == NULL)
    {
        _inputFile = fopen(_path.c_str(), "r");
    }
    return _inputFile != NULL;
}