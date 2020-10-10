#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <../meshData/su2Data.hpp>

class Su2Reader
{
private:
    std::string v_filename;

public:
    Su2Reader(std::string filename);
    ~Su2Reader();
};