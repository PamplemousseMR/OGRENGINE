#include "ScriptCompositor.hpp"

#include <iostream>

ScriptCompositor::ScriptCompositor() :
    FrameListener(),
    m_renderSystemLoader(),
    m_resourceLoader("../../config/resources.cfg"),
	m_log(nullptr),
	m_root(nullptr),
	m_sceneManager(nullptr),
	m_renderWindow(nullptr),
	m_viewport(nullptr),
	m_camera(nullptr),
    m_light(nullptr),
    m_cube(nullptr),
    m_cameraNode(nullptr)
{
	m_log = OGRE_NEW Ogre::LogManager();
    m_log->createLog("log.txt", false, false);

#ifdef _DEBUG
    m_root = OGRE_NEW::Ogre::Root("../../config/plugins_d.cfg");
#else
    m_root = OGRE_NEW ::Ogre::Root("../../config/plugins.cfg");
#endif
}

ScriptCompositor::~ScriptCompositor()
{
    if(m_root != nullptr) OGRE_DELETE m_root;
	if(m_log != nullptr) OGRE_DELETE m_log;
}

void ScriptCompositor::start()
{
	m_root->addFrameListener(this);

    m_renderSystemLoader.load(::RenderSystemManager::OPENGL3);
    m_renderSystemLoader.setColourDepth(::RenderSystemManager::DEPTH_32);
    m_renderSystemLoader.setDisplayFrequency(::RenderSystemManager::FREQUENCY_60);
    m_renderSystemLoader.setFSAA(::RenderSystemManager::FSAA_8);
    m_renderSystemLoader.setFullScreen(false);
    m_renderSystemLoader.setRTTPreferredMode(::RenderSystemManager::RTT_FBO);
    m_renderSystemLoader.setVSync(false);
    m_renderSystemLoader.setVSyncInterval(::RenderSystemManager::INTERVAL_1);
    m_renderSystemLoader.setVideoMode(::RenderSystemManager::MODE_800x600);

	loadScene();

    m_resourceLoader.load();
    m_resourceLoader.initialise("ScriptCompositor");

	createObject();

    ::Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, "script_compositor_C");
    ::Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_viewport, "script_compositor_C", true);
}

void ScriptCompositor::stop()
{
	destroyObject();

    m_resourceLoader.unload();

    unloadScene();

	m_root->removeFrameListener(this);
}

void ScriptCompositor::run()
{
    int i=0;
    while (++i < 10000)
    {
        if(m_renderWindow->isClosed())
        {
            return;
        }
        if(!m_root->renderOneFrame())
        {
            return;
        }
    }
}

void ScriptCompositor::loadScene()
{
	m_renderWindow = m_root->initialise(true, "OGRENGINE_RenderWindow");

	m_sceneManager = m_root->createSceneManager();
    m_sceneManager->setAmbientLight(::Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));

	m_camera = m_sceneManager->createCamera("OGRENGINE_Camera");
	m_camera->setNearClipDistance(Ogre::Real(0.01));
    m_camera->setFarClipDistance(Ogre::Real(1000));
	m_camera->setAutoAspectRatio(true);

	m_viewport = m_renderWindow->addViewport(m_camera);
	m_viewport->setBackgroundColour(Ogre::ColourValue(0.75, 0.75, 0.75));

	m_camera->setAspectRatio(Ogre::Real(m_viewport->getHeight() / m_viewport->getWidth()));

	m_cameraNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_cameraNode->attachObject(m_camera);
    m_cameraNode->setPosition(Ogre::Vector3(0, 0, 500));
}

void ScriptCompositor::unloadScene()
{
    m_lightNode->detachObject(m_light);
    m_sceneManager->getRootSceneNode()->removeChild(m_lightNode);

    m_cameraNode->detachObject(m_camera);
    m_sceneManager->getRootSceneNode()->removeChild(m_cameraNode);

    m_renderWindow->removeViewport(0);

	m_sceneManager->destroyCamera(m_camera);

    m_root->destroySceneManager(m_sceneManager);

    m_renderWindow->destroy();
	m_root->destroyRenderTarget(m_renderWindow);
}

void ScriptCompositor::createObject()
{
    // Light
	//----------------------------------
	{
        m_light = m_sceneManager->createLight("OGRENGINE_Light");
        m_light->setType(Ogre::Light::LT_POINT);
        m_light->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
        m_light->setSpecularColour(Ogre::ColourValue(1, 1, 1));

        m_lightNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
        m_lightNode->attachObject(m_light);
        m_lightNode->setPosition(Ogre::Vector3(500, 0, 1000));
	}
    // Cube
	//----------------------------------
	{
        ::Ogre::Entity* cube = m_sceneManager->createEntity("OGRENGINE_Cube", "cube.mesh", "ScriptCompositor");
        cube->setMaterialName("script_material_M");
		cube->setCastShadows(false);

        m_cube = m_sceneManager->getRootSceneNode()->createChildSceneNode();
        m_cube->attachObject(cube);
	}
}

void ScriptCompositor::destroyObject()
{
    // Cube
    //----------------------------------
    {
        ::Ogre::MovableObject* mov = m_cube->detachObject("OGRENGINE_Cube");
        m_sceneManager->getRootSceneNode()->removeChild(m_cube);
        m_sceneManager->destroyMovableObject(mov);
    }
    // Light
	//----------------------------------
	{
        ::Ogre::MovableObject* mov = m_sceneManager->getRootSceneNode()->detachObject("OGRENGINE_Light");
		m_sceneManager->destroyMovableObject(mov);
	}
}


// ================================= Events =================================

bool ScriptCompositor::frameStarted(const Ogre::FrameEvent&)
{
	return true;
}

bool ScriptCompositor::frameEnded(const Ogre::FrameEvent&)
{
	return true;
}

bool ScriptCompositor::frameRenderingQueued(const Ogre::FrameEvent&)
{
    // Rotate the cube
	//---------------------------------
	{
        m_cube->yaw(Ogre::Degree(Ogre::Real(0.02)));
        m_cube->pitch(Ogre::Degree(Ogre::Real(0.03)));
        m_cube->roll(Ogre::Degree(Ogre::Real(0.01)));
	}
	return true;
}
