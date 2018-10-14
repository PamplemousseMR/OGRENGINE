#include "Application.hpp"

#include <iostream>

int main()
{
    Application application;
    application.start();
    application.run();
    application.stop();

    std::cout << "[" <<__FUNCTION__ << "] " << "finished" << std::endl;

    return 0;
}
