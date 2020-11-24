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
        cout << "Element (" << i << "): Volume = " << meshData->getElement2Volumes()->at(i)
             << " Centre = (" << meshData->getElement2Centres()->at(2 * i) << ", " << meshData->getElement2Centres()->at(2 * i + 1) << ")"
             << " CV Projection = (" << meshData->getCVprojections()->at(2 * i) << ", " << meshData->getCVprojections()->at(2 * i + 1) << ")"
             << "\n";
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

void divergenceTest(MeshData *meshData) // Etat: Succés
{
    cout << "Début fonction divergenceTest()\n";
    vector<double> divergence;
    vector<double> champ = {1, -3};
    divergence.assign(meshData->getNELEM(), 0.);
    int L, R;
    for (int iFace = 0; iFace < meshData->getNFACE() - meshData->getNBOUNDARY(); iFace++)
    {
        L = meshData->getEsuf()->at(2 * iFace + 0);
        R = meshData->getEsuf()->at(2 * iFace + 1);
        divergence[L] += (champ[0] * meshData->getFace2Normales()->at(2 * iFace) + champ[1] * meshData->getFace2Normales()->at(2 * iFace + 1)) * meshData->getFace2Aires()->at(iFace);
        divergence[R] -= (champ[0] * meshData->getFace2Normales()->at(2 * iFace) + champ[1] * meshData->getFace2Normales()->at(2 * iFace + 1)) * meshData->getFace2Aires()->at(iFace);
    }
    for (int iFace = meshData->getNFACE() - meshData->getNBOUNDARY(); iFace < meshData->getNFACE(); iFace++)
    {
        L = meshData->getEsuf()->at(2 * iFace + 0);
        divergence[L] += (champ[0] * meshData->getFace2Normales()->at(2 * iFace) + champ[1] * meshData->getFace2Normales()->at(2 * iFace + 1)) * meshData->getFace2Aires()->at(iFace);
    }

    for (size_t i = 0; i < divergence.size(); i++)
    {
        cout << "Element " << i << " , divergence = " << divergence[i] << endl;
    }
    cout << "Fin fonction divegenceTest()\n";

    return;
}

void main_meshMetric_Test()
{
    MeshData *meshData = new MeshData();
    string path1 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/naca0012_euler_9x9x1_O_1B.su2";
    string path2 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/square_5x5.su2";
    string path3 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/NACA0012_65_mixed.su2";
    string path4 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/maillage_exemple_prof.su2";
    MeshRead meshRead = MeshRead(path1, meshData);
    meshRead.readFile();
    meshData->setConnectivity();
    MeshMetric meshMetric = MeshMetric(meshData);
    meshMetric.setMetric();
    metricElements(meshData);
    metricFaces(meshData);
    divergenceTest(meshData);
    delete meshData;

    return;
}