#pragma once

#include <filesystem>

#include <OGRE/OgreRoot.h>

class RootManager
{

public:

    static ::Ogre::Root* initialise(const std::filesystem::path& _path);
    static void clear();

private:

    std::filesystem::path m_path {};

};

