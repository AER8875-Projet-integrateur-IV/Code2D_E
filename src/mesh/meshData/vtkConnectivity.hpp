/* 
/Fichier vtkConnectivity.hpp
/Date: 16 octobre 2020 15h58
/Projet: Projet Intégrateur 4
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class VTKConnectivity
{
private:
    // Nombre de faces par éléments
    vector<int> _NFAEL{
        0,
        0, // VTK = 1 VERTEX
        0,
        0, // VTK = 3 LINE
        0,
        3, // TVTK = 5 TRIANGLE
        0,
        0,
        4, // VTK = 8 PIXEL
        4, // VTK = 9 QUAD
        4, // VTK = 10 TETRA
        6, // VTK = 11 VOXEL
        6, // VTK = 12 HEXAHEDRON
        5, // VTK = 13 WEDGE
        5, // VTK = 14 PYRAMID
    };
    vector<vector<int>> _NNOFA{
        {0},
        {0}, // VTK = 1 VERTEX
        {0},
        {0}, // VTK = 3 LINE
        {0},
        {2, 2, 2}, // TVTK = 5 TRIANGLE
        {0},
        {0},
        {2, 2, 2, 2},       // VTK = 8 PIXEL
        {2, 2, 2, 2},       // VTK = 9 QUAD
        {3, 3, 3, 3},       // VTK = 10 TETRA
        {4, 4, 4, 4, 4, 4}, // VTK = 11 VOXEL
        {4, 4, 4, 4, 4, 4}, // VTK = 12 HEXAHEDRON
        {3, 4, 3, 4, 4},    // VTK = 13 WEDGE
        {3, 3, 3, 3, 4},    // VTK = 14 PYRAMID
    };
    vector<vector<vector<int>>> _LPOFA{
        {{0}},
        {{0}}, // VTK = 1 VERTEX
        {{0}},
        {{0}}, // VTK = 3 LINE
        {{0}},
        {{0, 1}, {1, 2}, {2, 0}}, // TVTK = 5 TRIANGLE
        {{0}},
        {{0}},
        {{0, 1}, {1, 3}, {3, 2}, {2, 0}},                                                     // VTK = 8 PIXEL
        {{0, 1}, {1, 2}, {2, 3}, {3, 0}},                                                     // VTK = 9 QUAD
        {{0, 1, 3}, {2, 3, 1}, {0, 3, 2}, {1, 2, 0}},                                         // VTK = 10 TETRA
        {{0, 1, 5, 4}, {1, 3, 7, 5}, {7, 3, 2, 6}, {6, 2, 0, 4}, {4, 5, 7, 6}, {0, 2, 3, 1}}, // VTK = 11 VOXEL
        {{0, 1, 5, 4}, {1, 2, 6, 5}, {6, 2, 3, 7}, {7, 3, 0, 4}, {4, 5, 6, 7}, {0, 3, 2, 1}}, // VTK = 12 HEXAHEDRON
        {{0, 1, 2}, {2, 1, 4, 5}, {5, 4, 3}, {3, 0, 2, 5}, {0, 1, 4, 3}},                     // VTK = 13 WEDGE
        {{0, 1, 4}, {1, 2, 4}, {4, 2, 3}, {3, 0, 4}, {0, 1, 2, 3}},                           // VTK = 14 PYRAMID
    };

public:
    VTKConnectivity();
    ~VTKConnectivity();

    // Getters
    int getNFAEL(const int &vtkIndex);
    int getLnofa(const int &vtkIndex, const int &iFael);
    vector<int> getLpofa(const int &vtkIndex, const int &iFael);
};