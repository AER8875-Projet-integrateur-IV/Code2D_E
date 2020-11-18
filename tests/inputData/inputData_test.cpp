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

void verification_lecture(InputData inputData) // Etat: Succés
{
    cout << "\nDébut fonction verification_lecture()" << endl;
    inputData.readFile();

    cout << "Titre du projet: " << inputData.getTitle() << endl;
    cout << "Nombre de Mach: " << inputData.getMachNumber() << endl;
    cout << "angle d'attaque " << inputData.getAOA() << endl;
    cout << "Path du maillage: " << inputData.getMeshPath() << endl;
    cout << "Conditions limites 1 : " << inputData.getConditionsLimites()[0] << endl;
    cout << "Conditions limites 2 : " << inputData.getConditionsLimites()[1] << endl;

    cout << "Fin fonction verification_lecture()" << endl;
};

void main_inputData_Test()
{
    string path = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/inputData/Example.project";
    InputData inputData = InputData(path);
    openFile_test(inputData);
    verification_lecture(inputData);
    return;
}
