#pragma once

#include "ResourceManager.hpp"
#include "RenderSystemManager.hpp"

#include <OGRE/OgreFrameListener.h>

class ScriptCompositor : public Ogre::FrameListener
{

public:

    ScriptCompositor();
    virtual ~ScriptCompositor() override;

    void start();
	void stop();
	void run();

private:

    RenderSystemManager m_renderSystemLoader;
    ResourceManager m_resourceLoader;

	::Ogre::LogManager* m_log;
	::Ogre::Root* m_root;
	::Ogre::SceneManager* m_sceneManager;
	::Ogre::RenderWindow* m_renderWindow;

	::Ogre::Viewport* m_viewport;
	::Ogre::Camera* m_camera;

    ::Ogre::Light* m_light;
    ::Ogre::SceneNode* m_cube;

    ::Ogre::SceneNode* m_cameraNode;
    ::Ogre::SceneNode* m_lightNode;

	void loadScene();
	void createObject();

	void unloadScene();
	void destroyObject();

	// ================================= Events =================================

	bool frameStarted(const Ogre::FrameEvent &evt) override;
	bool frameEnded(const Ogre::FrameEvent &evt) override;
	bool frameRenderingQueued(const Ogre::FrameEvent& evt) override;
};
