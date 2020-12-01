/*
/Fichier meshMetric_test.cpp
/Date: 21 octobre 2020 - 04h20
/Projet: Projet Intégrateur 4
*/
#include "./postTecplot_test.hpp"

void main_postTecplot_Test()
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
    string extensionfichier = "./tests/post/test_path_2.dat";
    //SolWrite solWriter = SolWrite(extensionfichier, meshData);
    //solWriter.writeFile();
    delete meshData;

    return;
}
