#include "RenderSystemManager.hpp"

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSingleton.h>

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

    ::Ogre::Root& root = ::Ogre::Singleton< ::Ogre::Root >::getSingleton();
    const ::Ogre::RenderSystemList& rsList = root.getAvailableRenderers();

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

    m_renderSystem = *findedRSIt;

    std::cout << "[" << __FUNCTION__ << "] Render system options :" << std::endl;
    ::Ogre::ConfigOptionMap map = m_renderSystem->getConfigOptions();
	for (Ogre::ConfigOptionMap::const_iterator it = map.begin(); it != map.end(); it++)
	{
        std::cout << "[" << __FUNCTION__ << "]  - " << it->first << " :";
        for (size_t i = 0; i < it->second.possibleValues.size(); i++)
        {
			std::cout << "  " << it->second.possibleValues[i];
        }
		std::cout << std::endl;
    }

    root.setRenderSystem(m_renderSystem);
}

void RenderSystemManager::setColourDepth(RENDERSYSTEMMANAGER_DEPTH _depth)
{
    m_renderSystem->setConfigOption("Colour Depth", std::to_string(_depth));
}

void RenderSystemManager::setDisplayFrequency(RENDERSYSTEMMANAGER_FREQUENCY _freq)
{
    m_renderSystem->setConfigOption("Display Frequency", std::to_string(_freq));
}

void RenderSystemManager::setFSAA(RENDERSYSTEMMANAGER_FSAA _fsaa)
{
    m_renderSystem->setConfigOption("FSAA", std::to_string(_fsaa));
}

void RenderSystemManager::setFullScreen(bool _bool)
{
    m_renderSystem->setConfigOption("Full Screen", _bool ? "Yes" : "No");
}

void RenderSystemManager::setRTTPreferredMode(RENDERSYSTEMMANAGER_RTT _rtt)
{
    m_renderSystem->setConfigOption("RTT Preferred Mode", _rtt == RTT_FBO ? "FBO" : "");
}

void RenderSystemManager::setVSync(bool _bool)
{
    m_renderSystem->setConfigOption("VSync", _bool ? "Yes" : "No");
}

void RenderSystemManager::setVSyncInterval(RENDERSYSTEMMANAGER_INTERVAL _interval)
{
    m_renderSystem->setConfigOption("VSync Interval", std::to_string(_interval));
}

void RenderSystemManager::setVideoMode(RENDERSYSTEM_MODE _mode)
{
    std::string mode;
    switch(_mode)
    {
    case MODE_640x480 :
        mode = "640x480";
    break;
    case MODE_720x480 :
        mode = "720x480";
    break;
    case MODE_720x576 :
        mode = "720x576";
    break;
    case MODE_800x600 :
        mode = "800x600";
    break;
    case MODE_1024x768 :
        mode = "1024x768";
    break;
    case MODE_1152x864 :
        mode = "1152x864";
    break;
    case MODE_1176x664 :
        mode = "1176x664";
    break;
    case MODE_1280x720 :
        mode = "1280x720";
    break;
    case MODE_1280x768 :
        mode = "1280x768";
    break;
    case MODE_1280x800 :
        mode = "1280x800";
    break;
    case MODE_1280x960 :
        mode = "1280x960";
    break;
    case MODE_1280x024 :
        mode = "1280x024";
    break;
    case MODE_1360x768 :
        mode = "1360x768";
    break;
    case MODE_1366x768 :
        mode = "1366x768";
    break;
    case MODE_1440x480 :
        mode = "1440x480";
    break;
    case MODE_1440x900 :
        mode = "1440x900";
    break;
    case MODE_1600x900 :
        mode = "1600x900";
    break;
    case MODE_1600x024 :
        mode = "1600x024";
    break;
    case MODE_1680x050 :
        mode = "1680x050";
    break;
    case MODE_1768x992 :
        mode = "1768x992";
    break;
    case MODE_1920x080 :
        mode = "1920x080";
    break;
    }
    m_renderSystem->setConfigOption("Video Mode", mode);
}

void RenderSystemManager::setsRGBGammaConversion(bool _bool)
{
    m_renderSystem->setConfigOption("sRGB Gamma Conversion", _bool ? "Yes" : "No");
}
