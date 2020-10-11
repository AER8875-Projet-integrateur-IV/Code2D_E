#ifndef SU2READER_HPP
#define SU2READER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../meshData/meshData.hpp"
#include "../../tools/stringTools.hpp"

using namespace std;

class Su2Reader : public StringTools
{
private:
    string _path;
    shared_ptr<MeshData> _meshData;
    FILE *_inputFile;

    bool isFileValid();
    bool setIndice(string ligne, int &indice);
    void setParametres(string ligne, int indice);
    void setElement2Nodes(string ligne);
    void setElement2NodesFrontieres(string ligne);
    void setNodes(string ligne);

public:
    Su2Reader(string &path, shared_ptr<MeshData> meshData);
    ~Su2Reader();
    void readFile();
};
#endif