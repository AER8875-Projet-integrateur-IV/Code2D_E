/* 
/Fichier su2Reader.cpp
/Date: octobre 2020 
/Projet: Projet Intégrateur 4
*/

#include <./su2Reader.hpp>

Su2Reader::Su2Reader(string &path, shared_ptr<MeshData> meshData)
{
    _path = path;
    _meshData = meshData;
    _inputFile = NULL;
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
        //cin.get();
        exit(1);
    }
    cout << "Le fichier " << _path << " est bien ouvert." << endl;
    char *line = NULL;
    size_t len = 0;
    int indice = 0;
    int marker_index = 0;
    while ((getline(&line, &len, _inputFile)) != -1)
    {
        string ligne = line;
        ligne.pop_back(); // Supprime le caractère de fin de ligne /n.

        if (ligne[0] != '%')
        {
            // Indice de la zone à lire
            if (setIndice(ligne, indice, marker_index))
            {
                continue;
            }
            else
            {
                setParametres(ligne, indice, marker_index);
            }
        }

        //printf("%s", line);
    }
    if (line)
    {
        free(line);
    }
    cout << "Lecture du maillage terminée." << endl;

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

bool Su2Reader::setIndice(string ligne, int &indice, int &marker_index)
{
    if (ligne.find("NDIME") != string::npos)
    {
        indice = 0;
        string value = getEqualValue(ligne);
        _meshData->setNDIME(stoi(value));
    }
    else if (ligne.find("NELEM") != string::npos)
    {
        indice = 1;
        string value = getEqualValue(ligne);
        _meshData->setNELEM(stoi(value));
    }
    else if (ligne.find("NPOIN") != string::npos)
    {
        indice = 2;
        string value = getEqualValue(ligne);
        _meshData->setNPOIN(stoi(value));
    }
    else if (ligne.find("NMARK") != string::npos)
    {
        indice = 3;
        string value = getEqualValue(ligne);
        _meshData->setNMARK(stoi(value));
    }
    else if (ligne.find("MARKER_TAG") != string::npos)
    {
        indice = 4;
        string value = getEqualValue(ligne);
        _meshData->setMARKER_TAG(value);
        _meshData->setElement2NodesFrontieres();
    }
    else if (ligne.find("MARKER_ELEMS") != string::npos)
    {
        indice = 5;
        marker_index++;
        string value = getEqualValue(ligne);
        _meshData->setMARKER_ELEMS(stoi(value));
    }
    else
    {
        return false;
    }
    return true;
}

void Su2Reader::setParametres(string ligne, int indice, int marker_index)
{
    switch (indice)
    {
    case 0: // NDIME
        break;
    case 1: // NELEM
        setElement2Nodes(ligne);
        break;
    case 2: // NPOIN
        setNodes(ligne);
        break;
    case 3: // NMARK
        break;
    case 4: // MARKER_TAG
        break;
    case 5: // MARKER_ELEMS
        setElement2NodesFrontieres(ligne, marker_index);
        break;
    }
}

void Su2Reader::setElement2Nodes(string ligne)
{
    vector<string> elements = parseString(ligne, "\\s+");
    _meshData->setElement2Nodes(elements);
    return;
}

void Su2Reader::setNodes(string ligne)
{
    vector<string> noeuds = parseString(ligne, "\\s+");
    _meshData->setNodes(noeuds);
    return;
}

void Su2Reader::setElement2NodesFrontieres(string ligne, int marker_index)
{
    vector<string> elements = parseString(ligne, "\\s+");
    _meshData->setElement2NodesFrontieres(elements, marker_index);
    return;
}