#include "ManualMaterial.hpp"

#include <iostream>

int main()
{
    ManualMaterial application;
    application.start();
    application.run();
    application.stop();

    std::cout << "[" <<__FUNCTION__ << "] " << "finished" << std::endl;

    return 0;
}
