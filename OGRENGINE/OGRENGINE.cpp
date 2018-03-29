#include "stdafx.h"
#include "Application.h"

#include <iostream>

int main(int argc, char **argv)
{
	try {
		Application application;
		application.start();
		application.run();
		application.stop();
	}
	catch (Ogre::Exception &e) {
		std::cout << "[" << __FUNCTION__ << "] " << e.getFullDescription().c_str() << std::endl;
	}
	catch (std::exception &e) {
		std::cout << "[" << __FUNCTION__ << "] " << e.what() << std::endl;
	}

	std::cout << "[" <<__FUNCTION__ << "] " << "finished" << std::endl;
	getchar();

	return 0;
}