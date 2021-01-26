/* 
/Fichier vtkConnectivity.hpp
/Date: 16 octobre 2020 15h58
/Projet: Projet Intégrateur 4
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class VTKConnectivity
{
private:
    // Nombre de faces par éléments
    static vector<int> _NFAEL;
    // Nombre de noeuds par face
    static vector<vector<int>> _NNOFA;
    // Connectivité locale des faces
    static vector<vector<vector<int>>> _LPOFA;

public:
    VTKConnectivity();
    ~VTKConnectivity();

    // Getters
    static int getNfael(const int &vtkIndex);
    static int getLnofa(const int &vtkIndex, const int &iFael);
    static vector<int> getLpofa(const int &vtkIndex, const int &iFael);
};
