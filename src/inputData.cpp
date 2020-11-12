/* 
/Fichier inputData.cpp
/Date: 04 novembre 2020 - 21h34
/Projet: Projet Intégrateur 4
*/

#include "inputData.hpp"

InputData::InputData(string &path)
{
    _pathInputFile = path;
    _inputFile = NULL;
    return;
};

InputData::~InputData()
{
    if (_inputFile != NULL)
    {
        fclose(_inputFile);
    }
    return;
}

bool InputData::isFileValid()
{
    if (_inputFile == NULL)
    {
        _inputFile = fopen(_pathInputFile.c_str(), "r");
    }
    return _inputFile != NULL;
}

void InputData::readFile()
{
    if (!isFileValid())
    {
        // if there is a probleme when opening the file
        printf("Le programme n'arrive pas à ouvrir le fichier: %s\n", _pathInputFile.c_str());
        //cin.get();
        exit(1);
    }
    cout << "Le fichier " << _pathInputFile << " est bien ouvert." << endl;
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
    cout << "Lecture du fichier d'entrée terminée." << endl;

    return;
}

bool InputData::setIndice(string ligne, int &indice)
{
    if (ligne.find("TITRE") != string::npos)
    {
        indice = 0;
        _title = getEqualValue(ligne);
    }
    else if (ligne.find("MACH") != string::npos)
    {
        indice = 0;
        _Mach = stod(getEqualValue(ligne));
    }
    else if (ligne.find("ANGLE D'ATTAQUE") != string::npos)
    {
        indice = 0;
        _AOA = stod(getEqualValue(ligne));
    }
    else if (ligne.find("MAILLAGE") != string::npos)
    {
        indice = 0;
        _meshPath = getEqualValue(ligne);
    }
    else if (ligne.find("SCHEMA ESPACE") != string::npos)
    {
        indice = 0;
        _schemeSpace = getEqualValue(ligne);
    }
    else if (ligne.find("SCHEMA TEMPS") != string::npos)
    {
        indice = 0;
        _schemeTime = getEqualValue(ligne);
    }

    else if (ligne.find("DENSITE") != string::npos)
    {
        indice = 0;
        _density = stod(getEqualValue(ligne));
    }
    else if (ligne.find("PRESSION") != string::npos)
    {
        indice = 0;
        _pressure = stod(getEqualValue(ligne));
    }
    else if (ligne.find("INDICE ADIABATIQUE") != string::npos)
    {
        indice = 0;
        _ratioCpCv = stod(getEqualValue(ligne));
    }
    else if (ligne.find("VITESSE DU SON") != string::npos)
    {
        indice = 0;
        _soundSpeed = stod(getEqualValue(ligne));
    }
    else if (ligne.find("CFL") != string::npos)
    {
        indice = 0;
        _CFL = stod(getEqualValue(ligne));
    }
    else if (ligne.find("NOMBRE D'ITERATION") != string::npos)
    {
        indice = 0;
        _iterationMax = stoi(getEqualValue(ligne));
    }
    else if (ligne.find("CONDITIONS LIMITES") != string::npos)
    {
        indice = 1;
        int nombreCL = stoi(getEqualValue(ligne));
        _conditionsLimites.reserve(nombreCL);
    }
    else
    {
        return false;
    }
    return true;
}

void InputData::setParametres(string ligne, int indice)
{
    switch (indice)
    {
    case 0: // inline parameters
        break;
    case 1: // Conditions limites
        if (ligne.find("FARFIELD") != string::npos)
        {
            _conditionsLimites.push_back(0);
        }
        else if (ligne.find("WALL") != string::npos)
        {
            _conditionsLimites.push_back(1);
        }
        break;
    }
    return;
}

string InputData::getTitle() const
{
    return _title;
}

string InputData::getMeshPath() const
{
    return _meshPath;
}

double InputData::getMachNumber() const
{
    return _Mach;
}

double InputData::getAOA() const
{
    return _AOA;
}

double InputData::getSoundSpeed() const
{
    return _soundSpeed;
}

double InputData::getCFL() const
{
    return _CFL;
}

double InputData::getRatioCpCv() const
{
    return _ratioCpCv;
}

vector<int> InputData::getConditionsLimites() const
{
    return _conditionsLimites;
}

string InputData::getSchemeSpace() const
{
    return _schemeSpace;
}

string InputData::getSchemeTime() const
{
    return _schemeTime;
}

int InputData::getIterationMax() const
{
    return _iterationMax;
}