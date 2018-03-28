#pragma once

#include "Material.h"
#include "ResourceLoader.h"
#include "RenderSystemLoader.h"
#include "ViewportListener.h"
#include "Material.h"
#include "Compositor.h"

#include <Ogre.h>

#include <OISKeyboard.h>
#include <OISMouse.h>

class Application : public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener, public Ogre::WindowEventListener
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
	RenderSystemLoader m_renderSystemLoader;
	ViewportListener* m_viewportListener;
	Compositor* m_compositor;

	::Ogre::LogManager* m_log;
	::Ogre::Root* m_root;
	::Ogre::SceneManager* m_sceneManager;
	::Ogre::RenderWindow* m_renderWindow;

	Material m_material;

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

	::OIS::Mouse* m_mouse;
	::OIS::Keyboard* m_keyboard;
	::OIS::InputManager* m_inputManager;

	void loadScene();
	void initOIS();
	void createObject();

	void unloadScene();
	void finalizeOIS();
	void destroyObject();

	// ================================= Events =================================

	bool frameStarted(const Ogre::FrameEvent &evt) override;
	bool frameEnded(const Ogre::FrameEvent &evt) override;
	bool frameRenderingQueued(const Ogre::FrameEvent& evt) override;

	bool keyPressed(const OIS::KeyEvent &arg) override;
	bool keyReleased(const OIS::KeyEvent &arg) override;
	bool mouseMoved(const OIS::MouseEvent &arg) override;
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
};