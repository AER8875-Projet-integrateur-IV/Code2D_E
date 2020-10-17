/* 
/Fichier vtkConnectivity.cpp
/Date: 16 octobre 2020 16h14
/Projet: Projet Intégrateur 4
*/

#include "./vtkConnectivity.hpp"

VTKConnectivity::VTKConnectivity()
{
    return;
}

VTKConnectivity::~VTKConnectivity()
{
    return;
}

int VTKConnectivity::getNFAEL(const int &vtkIndex)
{
    return _NFAEL[vtkIndex];
}
int VTKConnectivity::getLnofa(const int &vtkIndex, const int &iFael)
{
    return _NNOFA[vtkIndex][iFael];
}

vector<int> VTKConnectivity::getLpofa(const int &vtkIndex, const int &iFael)
{
    return _LPOFA[vtkIndex][iFael];
}