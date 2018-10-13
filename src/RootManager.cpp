#include "RootManager.hpp"

#include <iostream>

::Ogre::Root* RootManager::initialise(const std::filesystem::path& _path)
{
    std::cout << "[" << __FUNCTION__ << "] ..." << std::endl;

    return OGRE_NEW::Ogre::Root(_path.string());
}

void RootManager::clear()
{
    OGRE_DELETE ::Ogre::Root::getSingletonPtr();
}
