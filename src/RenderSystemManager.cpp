#include "RenderSystemManager.hpp"

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSingleton.h>
#include <OGRE/OgreRenderSystem.h>

#include <iostream>

RenderSystemManager::RenderSystemManager()
{
}


RenderSystemManager::~RenderSystemManager()
{
}

void RenderSystemManager::load(RENDERSYSTEMMANAGER_TYPE _type)
{
    std::cout << "[" << __FUNCTION__ << "] ..." << std::endl;

    ::Ogre::Root* const root = ::Ogre::Singleton< ::Ogre::Root >::getSingletonPtr();
    const ::Ogre::RenderSystemList& rsList = root->getAvailableRenderers();

    std::for_each(rsList.begin(), rsList.end(), [&](const ::Ogre::RenderSystem* const _renderSystem)
    {
        std::cout << "[" << __FUNCTION__ << "] Render find : " << _renderSystem->getName() << std::endl;
    });

    std::string name;
    switch(_type)
    {
    case DIRECTX9:
        name = "Direct3D9";
        break;
    case DIRECTX11:
        name = "Direct3D11";
        break;
    case OPENGL:
        name = "OpenGL Rendering";
        break;
    case OPENGL3:
        name = "OpenGL 3+ Rendering";
        break;
    }

    ::Ogre::RenderSystemList::const_iterator findedRSIt = std::find_if(rsList.begin(), rsList.end(), [&](const ::Ogre::RenderSystem* const _renderSystem) -> bool
    {
        if(_renderSystem->getName().find(name))
        {
            return false;
        }
        return true;
    });


    if(findedRSIt == rsList.end())
    {
		throw std::exception("[Application::loadRenderSystem] can't find render system");
    }
    else
    {
        std::cout << "[" << __FUNCTION__ << "] Used render system : " << (*findedRSIt)->getName() << std::endl;
    }

    ::Ogre::RenderSystem* const findedRS = *findedRSIt;

    std::cout << "[" << __FUNCTION__ << "] Render system options :" << std::endl;
    ::Ogre::ConfigOptionMap map = findedRS->getConfigOptions();
	for (Ogre::ConfigOptionMap::const_iterator it = map.begin(); it != map.end(); it++)
	{
        std::cout << "[" << __FUNCTION__ << "]  - " << it->first << " :";
        for (size_t i = 0; i < it->second.possibleValues.size(); i++)
        {
			std::cout << "  " << it->second.possibleValues[i];
        }
		std::cout << std::endl;
	}

    findedRS->setConfigOption("Full Screen", "No");
    findedRS->setConfigOption("VSync", "No");
    findedRS->setConfigOption("Display Frequency", "60");
    findedRS->setConfigOption("FSAA", "16");

    root->setRenderSystem(findedRS);
}

void RenderSystemManager::unload()
{
    std::cout << "[" << __FUNCTION__ << "] ..." << std::endl;

    ::Ogre::Root* const root = ::Ogre::Singleton< ::Ogre::Root >::getSingletonPtr();
    root->setRenderSystem(nullptr);
}
