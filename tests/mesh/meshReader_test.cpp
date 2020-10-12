#include "./meshReader_test.hpp"

void openFile_test(MeshRead meshRead) // Etat: Succés
{
    cout << "\nDébut fonction openFile_test()" << endl;

    meshRead.readFile();
    cout << "Fin fonction openFile_test()" << endl;
};

void verification_lectureMaillage(MeshRead meshRead, shared_ptr<MeshData> meshData) // Etat: Succés
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
            cout << meshData->getNodes()[i * meshData->getNDIME() + j] << " ";
        }
        cout << "\n";
    }

    //Affichage des élements internes
    cout << "Affichage des éléments.\n";
    for (int i = 0; i < meshData->getNELEM(); i++)
    {
        cout << "Éléments " << i << " de type " << meshData->getElementTypes()[i] << ": ";
        for (int j = meshData->getElement2NodesStart()[i]; j < meshData->getElement2NodesStart()[i + 1]; j++)
        {
            cout << meshData->getElement2Nodes()[j] << " ";
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
            cout << "Boundary Éléments " << j << " de type " << meshData->getElementTypesBoundary()[i][j] << ": ";
            for (int k = meshData->getElement2NodesStartBoundary()[i][j]; k < meshData->getElement2NodesStartBoundary()[i][j + 1]; k++)
            {
                cout << meshData->getElement2NodesBoundary()[i][k] << " ";
            }
            cout << "\n";
        }
        cout << "\n\n";
    }
    cout << "Fin fonction verification_lectureMaillage()" << endl;
};

void main_Test()
{
    shared_ptr<MeshData> meshData = make_shared<MeshData>();
    string path1 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/naca0012_euler_9x9x1_O_1B.su2";
    string path2 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/square_5x5.su2";
    string path3 = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/exemple_mesh/NACA0012_65_mixed.su2";
    MeshRead meshRead = MeshRead(path3, meshData);
    openFile_test(meshRead);
    verification_lectureMaillage(meshRead, meshData);
    return;
}