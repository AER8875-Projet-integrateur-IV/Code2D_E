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
    cout << "Le fichier " << _path << "est bien ouvert." << endl;
    char *line = NULL;
    size_t len = 0;
    int indice = 0;
    while ((getline(&line, &len, _inputFile)) != -1)
    {
        string ligne = line;
        ligne.pop_back(); // Supprime le caractère de fin de ligne /n.

        if (ligne[0] != '%')
        {
            // Indice de la zone à lire
            if (setIndice(ligne, indice))
            {
                continue;
            }
            else
            {
                setParametres(ligne, indice);
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

bool Su2Reader::setIndice(string ligne, int &indice)
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
    }
    else if (ligne.find("MARKER_ELEMS") != string::npos)
    {
        indice = 5;
        string value = getEqualValue(ligne);
        _meshData->setMARKER_ELEMS(stoi(value));
    }
    else
    {
        return false;
    }
    return true;
}

void Su2Reader::setParametres(string ligne, int indice)
{
    switch (indice)
    {
    case 0: // NDIME
        break;
    case 1: // NELEM
        setElemen2Nodes(ligne);
        break;
    case 2: // NPOIN
        setNodes(ligne);
        break;
    case 3: // NMARK
        break;
    case 4: // MARKER_TAG
        setRefLoadFile(line);
        break;
    case Param::controlFile:
        setControlFile(line);
        break;
    case Param::missionDefFile:
        setMissionDefFile(line);
        break;
    case Param::missionMix:
        setMissionMix(line);
        break;
    case Param::rangPair:
        setRangPair(line);
        break;
    case Param::user:
        setUser(line);
        break;
    case Param::target:
        setTarget(line);
        break;
    case Param::outputName:
        setOutputName(line);
        break;
    case Param::crackGrowth:
        //setCrackGrowth(line);
        break;
    default:
        printf("ERROR: unknown element in projectFile\n");
        system("pause");
        exit(1);
    }
}

bool Su2Reader::isFileValid()
{
    if (_inputFile == NULL)
    {
        _inputFile = fopen(_path.c_str(), "r");
    }
    return _inputFile != NULL;
}