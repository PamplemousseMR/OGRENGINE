#pragma once

#include "ResourceLoader.hpp"
#include "RenderSystemManager.hpp"

#include <OGRE/Ogre.h>

class Application : public Ogre::FrameListener
{

public:

	Application();
	~Application();

	void start() throw(...);
	void stop();
	void run();

private:

	static const float s_TRANSLATE_SCALE;
	static const float s_ROTATE_SCALE;

	ResourceLoader m_resourceLoader;
	RenderSystemManager m_renderSystemLoader;

	::Ogre::LogManager* m_log;
	::Ogre::Root* m_root;
	::Ogre::SceneManager* m_sceneManager;
	::Ogre::RenderWindow* m_renderWindow;

	::Ogre::Viewport* m_viewport;
	::Ogre::Camera* m_camera;
	::Ogre::Light* m_light1;
	::Ogre::Light* m_light2;
	::Ogre::SceneNode* m_cube1;
	::Ogre::SceneNode* m_cube2;
	::Ogre::SceneNode* m_cube3;
	::Ogre::SceneNode* m_cube4;
	::Ogre::SceneNode* m_sphere1;

	::Ogre::SceneNode* m_cameraNode;
	::Ogre::SceneNode* m_cameraYawNode;
	::Ogre::SceneNode* m_cameraPitchNode;
	::Ogre::SceneNode* m_cameraRollNode;
	::Ogre::Vector3 m_translation;
	::Ogre::Vector2 m_rotation;

	void loadScene();
	void createObject();

	void unloadScene();
	void destroyObject();

	// ================================= Events =================================

	bool frameStarted(const Ogre::FrameEvent &evt) override;
	bool frameEnded(const Ogre::FrameEvent &evt) override;
	bool frameRenderingQueued(const Ogre::FrameEvent& evt) override;
};
