#include "ResourceManager.hpp"

#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreResourceGroupManager.h>

ResourceManager::ResourceManager(const std::filesystem::path& _path) :
    m_path(_path)
{
}


ResourceManager::~ResourceManager()
{
}

void ResourceManager::load()
{
    std::cout << "[" << __FUNCTION__ << "] ..." << std::endl;

    ::Ogre::ResourceGroupManager& manager = ::Ogre::ResourceGroupManager::getSingleton();
    manager.setLoadingListener(&m_listener);

	::Ogre::ConfigFile configFile;
    configFile.load(m_path.string());

    const ::Ogre::ConfigFile::SettingsBySection_ settings = configFile.getSettingsBySection();
	::Ogre::String resourceGroupName, typeName, archName;
    for (const auto& seci : settings)
	{
		resourceGroupName = seci.first;
		Ogre::ConfigFile::SettingsMultiMap settings = seci.second;

		Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i=settings.begin() ; i!=settings.end() ; ++i)
		{
			typeName = i->first;
            archName = i->second;
            manager.addResourceLocation(archName, typeName, resourceGroupName);
		}
	}
}

void ResourceManager::unload()
{
    std::cout << "[" << __FUNCTION__ << "] ..." << std::endl;

    ::Ogre::ResourceGroupManager& manager = ::Ogre::ResourceGroupManager::getSingleton();
    manager.setLoadingListener(&m_listener);

    ::Ogre::ConfigFile configFile;
    configFile.load(m_path.string());

    const ::Ogre::ConfigFile::SettingsBySection_ settings = configFile.getSettingsBySection();
    ::Ogre::String resourceGroupName, typeName;
    for (const auto& seci : settings)
    {
        resourceGroupName = seci.first;
        Ogre::ConfigFile::SettingsMultiMap settings = seci.second;

        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings.begin(); i != settings.end(); ++i)
        {
            typeName = i->first;
            manager.removeResourceLocation(typeName, resourceGroupName);
        }
    }
}

void ResourceManager::initialiseAll() const
{
    ::Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void ResourceManager::initialise(const ::Ogre::String& _group) const
{
    ::Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(_group);
}

void ResourceManager::clearAll() const
{
    ::Ogre::ResourceGroupManager& manager = ::Ogre::ResourceGroupManager::getSingleton();
    for(::Ogre::String group : manager.getResourceGroups())
    {
        manager.clearResourceGroup(group);
    }
}

void ResourceManager::clear(const ::Ogre::String& _group) const
{
    ::Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(_group);
}

