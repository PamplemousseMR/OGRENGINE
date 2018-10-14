#pragma once

#include <OGRE/OgreString.h>
#include <OGRE/OgreMaterial.h>
#include <OGRE/OgreResourceGroupManager.h>

class Material
{

public:

    Material(const std::string&, const std::string& = ::Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    ~Material();

    void load();
    void unload();

private:

    ::Ogre::String m_name;
    ::Ogre::String m_group;
    ::Ogre::MaterialPtr m_material;

};
