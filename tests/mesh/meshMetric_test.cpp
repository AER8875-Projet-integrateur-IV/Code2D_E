/*
/Fichier meshMetric_test.cpp
/Date: 21 octobre 2020 - 04h20
/Projet: Projet Intégrateur 4
*/
#include "./meshMetric_test.hpp"

void metricElements(MeshData *meshData) // Etat: Probleme avec le signe des aires
{
    cout << "Début fonction metricElements()\n";
    for (int i = 0; i < meshData->getNELEM(); i++)
    {
        cout << "Element (" << i << "): Volume = " << meshData->getElement2Volumes()->at(i);
        cout << " Centre = (" << meshData->getElement2Centres()->at(2 * i) << ", " << meshData->getElement2Centres()->at(2 * i + 1);
        cout << ")\n";
    }
    cout << "Fin fonction metricElements()\n";
    return;
}

void metricFaces(MeshData *meshData) // Etat: Succés
{
    cout << "Début fonction metricFaces()\n";
    for (int i = 0; i < meshData->getNFACE(); i++)
    {
        cout << "Face (" << i << "): Aire = " << meshData->getFace2Aires()->at(i);
        cout << " Centre = (" << meshData->getFace2Centres()->at(2 * i) << ", " << meshData->getFace2Centres()->at(2 * i + 1);
        cout << ") Normale = (" << meshData->getFace2Normales()->at(2 * i) << ", " << meshData->getFace2Normales()->at(2 * i + 1);
        cout << ")\n";
    }
    cout << "Fin fonction metricFaces()\n";
    return;
}

void main_meshMetric_Test()
{
    MeshData *meshData = new MeshData();
    string path1 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/naca0012_euler_9x9x1_O_1B.su2";
    string path2 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/square_5x5.su2";
    string path3 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/NACA0012_65_mixed.su2";
    string path4 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/maillage_exemple_prof.su2";
    string path5 = "/home/vincentbar/Documents/square_5x5.su2";
    MeshRead meshRead = MeshRead(path5, meshData);
    meshRead.readFile();
    meshData->setConnectivity();
    MeshMetric meshMetric = MeshMetric(meshData);
    meshMetric.setMetric();
    metricElements(meshData);
    metricFaces(meshData);
    delete meshData;

    return;
}
