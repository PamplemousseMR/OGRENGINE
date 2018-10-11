#include "ResourceLoader.h"

#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreResourceGroupManager.h>

#include <iostream>

ResourceLoader::ResourceLoader(const std::string& _path)
	: m_path(_path)
{
}


ResourceLoader::~ResourceLoader()
{
}

void ResourceLoader::start()
{
	::Ogre::ConfigFile configFile;

	configFile.load(m_path);

	const ::Ogre::ConfigFile::SettingsBySection_ secis = configFile.getSettingsBySection();
	::Ogre::String resourceGroupName, typeName, archName;
	for (const auto& seci : secis)
	{
		resourceGroupName = seci.first;
		Ogre::ConfigFile::SettingsMultiMap settings = seci.second;
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings.begin(); i != settings.end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			std::cout << "[" << __FUNCTION__ << "] " << "Resource find : " << typeName << " " << archName << " " << resourceGroupName << std::endl;
			::Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, resourceGroupName);
		}
	}
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void ResourceLoader::stop()
{
	::Ogre::ConfigFile configFile;

	configFile.load(m_path);

	const ::Ogre::ConfigFile::SettingsBySection_ secis = configFile.getSettingsBySection();
	::Ogre::String resourceGroupName, typeName;
	for (const auto& seci : secis)
	{
		resourceGroupName = seci.first;
		Ogre::ConfigFile::SettingsMultiMap settings = seci.second;
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings.begin(); i != settings.end(); ++i)
		{
			typeName = i->first;
			::Ogre::ResourceGroupManager::getSingleton().removeResourceLocation(typeName, resourceGroupName);
		}
	}
}
