/* 
/Fichier inputData_test.cpp
/Date: 04 novembre 2020 - 22h34
/Projet: Projet Intégrateur 4
*/

#include "./inputData_test.hpp"

void openFile_test(InputData inputData) // Etat: Succés
{
    cout << "\nDébut fonction openFile_test()" << endl;

    inputData.readFile();
    cout << "Fin fonction openFile_test()" << endl;
};

/* void verification_lecture(InputData inputData) // Etat: Succés
{
    cout << "\nDébut fonction verification_lecture()" << endl;
    inputData.readFile();

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
}; */

void main_inputData_Test()
{
    string path = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/inputData/Example.project";
    InputData inputData = InputData(path);
    openFile_test(inputData);
    //verification_lectureMaillage(inputData);
    return;
}
