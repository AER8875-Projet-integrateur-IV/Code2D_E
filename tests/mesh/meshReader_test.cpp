#include "./meshReader_test.hpp"

void openFile_test() // Etat: Succés
{
    cout << "Début fonction openFile_test()" << endl;
    shared_ptr<MeshData> meshData = make_shared<MeshData>();
    string path = "/home/aziz/Bureau/A2020/PI4/Codes/Code2D_E/tests/mesh/naca0012_euler_9x9x1_O_1B.su2";

    MeshRead meshRead = MeshRead(path, meshData);
    meshRead.readFile();
    cout << "Fin fonction openFile_test()" << endl;
};
