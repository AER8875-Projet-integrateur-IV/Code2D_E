/* 
/Fichier meshConnectivity_test.cpp
/Date: 12 octobre 2020 - 20h56
/Projet: Projet Intégrateur 4
*/
#include "./meshConnectivity_test.hpp"

void connectiviteNodeElements(shared_ptr<MeshData> meshData) // Etat: Succés
{
    cout << "Début fonction connectiviteNodeElements()\n";
    for (int i = 1; i < meshData->getNPOIN() + 1; i++)
    {
        cout << "EsupStart " << meshData->getEsupStart()[i] << " (" << i - 1 << ")\n";
        for (int j = meshData->getEsupStart()[i - 1]; j < meshData->getEsupStart()[i]; j++)
        {
            cout << meshData->getEsup()[j] << " ";
        }
        cout << "\n";
    }
    cout << "Fin fonction connectiviteNodeElements()\n";
    return;
}

void connectiviteFaces(shared_ptr<MeshData> meshData) // Etat: Succés
{
    cout << "Début fonction connectiviteFaces()\n";
    for (size_t i = 0; i < meshData->getNFAEL().size(); i++)
    {
        int nfaces = meshData->getNFAEL()[i];
        cout << "Elements (" << i << ") Nbre de Faces: " << nfaces << "\n";
        for (int j = 0; j < nfaces; j++)
        {
            int nnoeuds = meshData->getNNOFA()[i][j];
            cout << "\tNbre de Noeuds: " << nnoeuds << "\n";
            cout << "\t\tNoeuds: ";
            for (int k = 0; k < nnoeuds; k++)
            {
                cout << meshData->getLpofa()[i][j][k] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
    cout << "Fin fonction connectiviteFaces()\n";
    return;
}

void connectiviteElementElements(shared_ptr<MeshData> meshData) // Etat: Succés
{
    cout << "Début fonction connectiviteElementElements()\n";
    for (int i = 1; i < meshData->getNELEM() + 1; i++)
    {
        cout << "EsuelStart " << meshData->getEsuelStart()[i] << " (" << i - 1 << ")\n";
        for (int j = meshData->getEsuelStart()[i - 1]; j < meshData->getEsuelStart()[i]; j++)
        {
            cout << meshData->getEsuel()[j] << " ";
        }
        cout << "\n";
    }
    cout << "Fin fonction connectiviteElementElements()\n";
    return;
}

void main_meshConnectivity_Test()
{
    shared_ptr<MeshData> meshData = make_shared<MeshData>();
    string path1 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/naca0012_euler_9x9x1_O_1B.su2";
    string path2 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/square_5x5.su2";
    string path3 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/NACA0012_65_mixed.su2";
    string path4 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/maillage_exemple_prof.su2";
    MeshRead meshRead = MeshRead(path4, meshData);
    meshRead.readFile();
    meshData->setConnectivity();
    //connectiviteNodeElements(meshData);
    //connectiviteFaces(meshData);
    connectiviteElementElements(meshData);

    return;
}