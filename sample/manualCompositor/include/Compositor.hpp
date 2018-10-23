#pragma once

#include <OGRE/OgreString.h>
#include <OGRE/OgreResourceGroupManager.h>

class Compositor
{

public:

    Compositor(const std::string&, const std::string& = ::Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    ~Compositor();

    void load();
    void unload();

private:

    ::Ogre::String m_name;
    ::Ogre::String m_group;
    ::Ogre::CompositorPtr m_compositor;

};
