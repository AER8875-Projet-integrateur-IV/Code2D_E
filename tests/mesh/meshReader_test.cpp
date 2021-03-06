#include "./meshReader_test.hpp"

void openFile_test(MeshRead meshRead) // Etat: Succés
{
    cout << "\nDébut fonction openFile_test()" << endl;

    meshRead.readFile();
    cout << "Fin fonction openFile_test()" << endl;
};

void verification_lectureMaillage(MeshRead meshRead, MeshData *meshData) // Etat: Succés
{
    cout << "\nDébut fonction verification_lectureMaillage()" << endl;
    meshRead.readFile();

    // Affichage des noeuds:
    cout << "Affichage des noeuds.\n";
    for (int i = 0; i < meshData->getNPOIN(); i++)
    {
        cout << "Noeuds " << i << " ";
        for (int j = 0; j < meshData->getNDIME(); j++)
        {
            cout << meshData->getNodes()->at(i * meshData->getNDIME() + j) << " ";
        }
        cout << "\n";
    }

    //Affichage des élements internes
    cout << "Affichage des éléments.\n";
    for (int i = 0; i < meshData->getNELEM(); i++)
    {
        cout << "Éléments " << i << " de type " << meshData->getElementTypes()->at(i) << ": ";
        for (int j = meshData->getElement2NodesStart()->at(i); j < meshData->getElement2NodesStart()->at(i + 1); j++)
        {
            cout << meshData->getElement2Nodes()->at(j) << " ";
        }
        cout << "\n";
    }

    //Affichage des élements frontières
    cout << "Affichage des éléments Frontières.\n";
    for (int i = 0; i < meshData->getNMARK(); i++)
    {
        cout << "Marker_TAG " << i << " - " << meshData->getMARKER_TAG()[i] << ":\n";
        for (int j = 0; j < meshData->getMARKER_ELEMS()[i]; j++)
        {
            cout << "Boundary Éléments " << j << " de type " << meshData->getElementTypesBoundary()->at(i)[j] << ": ";
            for (int k = meshData->getElement2NodesStartBoundary()->at(i)[j]; k < meshData->getElement2NodesStartBoundary()->at(i)[j + 1]; k++)
            {
                cout << meshData->getElement2NodesBoundary()->at(i)[k] << " ";
            }
            cout << "\n";
        }
        cout << "\n\n";
    }
    cout << "Fin fonction verification_lectureMaillage()" << endl;
};

void main_meshReader_Test()
{
    MeshData *meshData = new MeshData();
    string path1 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/naca0012_euler_9x9x1_O_1B.su2";
    string path2 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/square_5x5.su2";
    string path3 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/NACA0012_65_mixed.su2";
    MeshRead meshRead = MeshRead(path1, meshData);
    openFile_test(meshRead);
    verification_lectureMaillage(meshRead, meshData);
    delete meshData;
    return;
}