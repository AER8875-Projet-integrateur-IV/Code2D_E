/* 
/Fichier meshMetric_test.cpp
/Date: 21 octobre 2020 - 04h20
/Projet: Projet Intégrateur 4
*/
#include "./meshMetric_test.hpp"

void metricElements(shared_ptr<MeshData> meshData) // Etat: Probleme avec le signe des aires
{
    cout << "Début fonction metricElements()\n";
    for (int i = 0; i < meshData->getNELEM(); i++)
    {
        cout << "Element (" << i << "): Volume = " << meshData->getElement2Volumes()[i];
        cout << " Centres = (" << meshData->getElement2Centres()[2 * i] << ", " << meshData->getElement2Centres()[2 * i];
        cout << ")\n";
    }
    cout << "Fin fonction metricElements()\n";
    return;
}

void metricFaces(shared_ptr<MeshData> meshData) // Etat: Succés
{
    cout << "Début fonction metricFaces()\n";
    /*     for (int iElem = 0; iElem < meshData->getNELEM(); iElem++)
    {
        int nfaces = meshData->getNfael(iElem);
        cout << "Elements (" << iElem << ") Nbre de Faces: " << nfaces << "\n";
        for (int iFael = 0; iFael < nfaces; iFael++)
        {
            int nnoeuds = meshData->getLnofa(iElem, iFael);
            cout << "\tNbre de Noeuds: " << nnoeuds << "\n";
            cout << "\t\tNoeuds: ";
            vector<int> Noeuds = meshData->getLpofa(iElem, iFael);
            for (int iNoeud = 0; iNoeud < nnoeuds; iNoeud++)
            {
                cout << Noeuds[iNoeud] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    } */
    cout << "Fin fonction metricFaces()\n";
    return;
}

void main_meshMetric_Test()
{
    shared_ptr<MeshData> meshData = make_shared<MeshData>();
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

    return;
}