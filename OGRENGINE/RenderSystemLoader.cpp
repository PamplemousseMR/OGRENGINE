#include "stdafx.h"
#include "RenderSystemLoader.h"

#include <OgreRoot.h>
#include <OgreSingleton.h>
#include <OgreRenderSystem.h>

#include <iostream>

RenderSystemLoader::RenderSystemLoader()
{
}


RenderSystemLoader::~RenderSystemLoader()
{
}

void RenderSystemLoader::start() 
{
	std::cout << "[" << __FUNCTION__ << "] " << "Start" << std::endl;
	::Ogre::Root* root = ::Ogre::Singleton< ::Ogre::Root >::getSingletonPtr();
	const Ogre::RenderSystemList& rsList = root->getAvailableRenderers();
	Ogre::RenderSystem* rs = nullptr;

	for (::Ogre::RenderSystemList::const_iterator it = rsList.begin(); it != rsList.end(); ++it)
	{
		std::cout << "[" << __FUNCTION__ << "] " << "Render find : " << (*it)->getName() << std::endl;
		if ((*it)->getName().find("OpenGL") != Ogre::String::npos)
		{
			rs = *it;
			break;
		}
	}

	if (rs == nullptr)
		throw std::exception("[Application::loadRenderSystem] can't find render system");

	// Diplay all configurations
	Ogre::ConfigOptionMap map = rs->getConfigOptions();
	for (Ogre::ConfigOptionMap::const_iterator it = map.begin(); it != map.end(); it++)
	{
		std::cout << "[" << __FUNCTION__ << "] " << it->first << " :";
		for (int i = 0; i < it->second.possibleValues.size(); i++)
			std::cout << "  " << it->second.possibleValues[i];
		std::cout << std::endl;
	}

	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("VSync", "No");
	rs->setConfigOption("Display Frequency", "60");
	rs->setConfigOption("FSAA", "16");

	root->setRenderSystem(rs);
}

void RenderSystemLoader::stop()
{
	std::cout << "[" << __FUNCTION__ << "] " << "Stop" << std::endl;
}