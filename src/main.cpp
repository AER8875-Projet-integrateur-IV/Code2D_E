#include <iostream>
#include "../tests/mesh/meshMetric_test.hpp"
#include "../tests/mesh/meshConnectivity_test.hpp"
#include "../tests/inputData/inputData_test.hpp"
#include "../tests/post/postTecplot_test.hpp"

int main(int argc, char const *argv[])
{
    std::cout << "Hello Word!" << std::endl;
    //main_meshConnectivity_Test();
    //main_meshMetric_Test();
    //main_inputData_Test();
    main_postTecplot_Test();
    return 0;
}
