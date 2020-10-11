#include "./meshReader_test.hpp"

void openFile_test(MeshRead meshRead) // Etat: Succés
{
    cout << "\nDébut fonction openFile_test()" << endl;

    meshRead.readFile();
    cout << "Fin fonction openFile_test()" << endl;
};

void verification_lectureMaillage(MeshRead meshRead, shared_ptr<MeshData> meshData) // Etat:
{
    cout << "\nDébut fonction verification_lectureMaillage()" << endl;
    meshRead.readFile();
    // Affichage des noeuds:

    cout << "Fin fonction verification_lectureMaillage()" << endl;
};

void main_Test()
{
    shared_ptr<MeshData> meshData = make_shared<MeshData>();
    string path = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/naca0012_euler_9x9x1_O_1B.su2";

    MeshRead meshRead = MeshRead(path, meshData);
    openFile_test(meshRead);
    verification_lectureMaillage(meshRead, meshData);
    return;
}