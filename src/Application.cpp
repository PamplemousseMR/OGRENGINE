#include "Application.hpp"

#include <iostream>

const float Application::s_TRANSLATE_SCALE = 1.f;
const float Application::s_ROTATE_SCALE = 0.005f;

Application::Application() :
	m_log(nullptr),
	m_root(nullptr),
	m_sceneManager(nullptr),
	m_renderWindow(nullptr),
	m_viewport(nullptr),
	m_camera(nullptr),
	m_light1(nullptr),
	m_light2(nullptr),
	m_cube1(nullptr),
	m_cube2(nullptr),
	m_cameraNode(nullptr),
	m_cameraYawNode(nullptr),
	m_cameraPitchNode(nullptr),
	m_cameraRollNode(nullptr),
	m_translation(Ogre::Vector3::ZERO),
	m_rotation(::Ogre::Vector2::ZERO),
    m_resourceLoader("config/resources_d.cfg"),
    m_renderSystemLoader()
{
	FrameListener();
	m_log = OGRE_NEW Ogre::LogManager();
    m_log->createLog("log.txt", false, false);
}

Application::~Application()
{
	if(m_root != nullptr) OGRE_DELETE m_root;
	if(m_log != nullptr) OGRE_DELETE m_log;
}

void Application::run()
{
	while (true)
	{
		if (m_renderWindow->isClosed())
			return;

		if (!m_root->renderOneFrame())
			return;
	}
}

void Application::start()
{
#ifdef _DEBUG
    m_root = OGRE_NEW::Ogre::Root("config/plugins_d.cfg");
#else
	m_root = OGRE_NEW ::Ogre::Root("Config\\plugins.cfg");
#endif

	m_root->addFrameListener(this);

    m_renderSystemLoader.load(::RenderSystemManager::OPENGL3);
	loadScene();
	m_resourceLoader.start();

	::Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, "blackAndWhite_C");
    ::Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_viewport, "blackAndWhite_C", true);

    ::Ogre::ResourceGroupManager::ResourceManagerIterator it = ::Ogre::ResourceGroupManager::getSingleton().getResourceManagerIterator();
    while (it.hasMoreElements())
    {
        ::Ogre::ResourceManager::ResourceMapIterator resourceIt = it.getNext()->getResourceIterator();
        while (resourceIt.hasMoreElements())
        {
            ::Ogre::SharedPtr< ::Ogre::Resource >  resource = resourceIt.getNext();
            std::cout << "========================== " << resource->getName() << " " << resource->getGroup() << std::endl;
        }
    }

	createObject();
}

void Application::stop()
{
	destroyObject();

	unloadScene();

    m_renderSystemLoader.unload();
	m_resourceLoader.stop();

	m_root->removeFrameListener(this);
}

void Application::loadScene()
{
	m_renderWindow = m_root->initialise(true, "OGRENGINE_RenderWindow");

	m_sceneManager = m_root->createSceneManager();
	m_sceneManager->setAmbientLight(Ogre::ColourValue(0.1f, 0.1f, 0.1f));
	m_sceneManager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);

	m_camera = m_sceneManager->createCamera("OGRENGINE_Camera");
    m_camera->setPosition(Ogre::Vector3(0, 300, 500));
	m_camera->setNearClipDistance(Ogre::Real(0.01));
	m_camera->setFarClipDistance(Ogre::Real(1000));
	m_camera->setAutoAspectRatio(true);

	m_viewport = m_renderWindow->addViewport(m_camera);
	m_viewport->setBackgroundColour(Ogre::ColourValue(0.75, 0.75, 0.75));

	m_camera->setAspectRatio(Ogre::Real(m_viewport->getHeight() / m_viewport->getWidth()));

	m_cameraNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
	m_cameraYawNode = m_cameraNode->createChildSceneNode();
	m_cameraPitchNode = m_cameraYawNode->createChildSceneNode();
	m_cameraRollNode = m_cameraPitchNode->createChildSceneNode();

	m_cameraRollNode->attachObject(m_camera);

	::Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(::Ogre::TFO_ANISOTROPIC);
	::Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(32);
}

void Application::unloadScene()
{
	m_cameraRollNode->detachObject(m_camera);
	m_cameraPitchNode->removeChild(m_cameraRollNode);
	m_cameraYawNode->removeChild(m_cameraPitchNode);
	m_sceneManager->getRootSceneNode()->removeChild(m_cameraYawNode);
	m_renderWindow->removeAllViewports();
	m_renderWindow->destroy();
	m_sceneManager->destroyCamera(m_camera);
	m_sceneManager->clearScene();
	m_root->destroySceneManager(m_sceneManager);
	m_root->destroyRenderTarget(m_renderWindow);
}

void Application::createObject()
{
	// Light1
	//----------------------------------
	{
		m_light1 = m_sceneManager->createLight("OGRENGINE_Light1");
		m_light1->setType(Ogre::Light::LT_SPOTLIGHT);
		m_light1->setPosition(Ogre::Vector3(0,1000,0));
		m_light1->setDirection(Ogre::Vector3(0, -1, 0));
		m_light1->setSpotlightRange(Ogre::Degree(Ogre::Real(100)), Ogre::Degree(Ogre::Real(40)), 0.1);
		m_light1->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
		m_light1->setSpecularColour(Ogre::ColourValue(1, 1, 1));
        m_sceneManager->getRootSceneNode()->attachObject(m_light1);
	}
	// Light2
	//----------------------------------
	{
		m_light2 = m_sceneManager->createLight("OGRENGINE_Light2");
		m_light2->setType(Ogre::Light::LT_DIRECTIONAL);
		m_light2->setDirection(Ogre::Vector3(1, -1, -1));
		m_light2->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
		m_light2->setSpecularColour(Ogre::ColourValue(1, 1, 1));
        m_sceneManager->getRootSceneNode()->attachObject(m_light2);
	}
	// Cube1
	//----------------------------------
	{
        ::Ogre::Entity* cube = m_sceneManager->createEntity("OGRENGINE_NodeCube1", "cube.mesh", "General");
        cube->setMaterialName("doubleTexture_M");
		cube->setCastShadows(false);

		m_cube1 = m_sceneManager->getRootSceneNode()->createChildSceneNode();
		m_cube1->translate(Ogre::Vector3(100, 250, 100));
        m_cube1->attachObject(cube);
	}
	// Cube2
	//----------------------------------
	{
        ::Ogre::Entity* cube = m_sceneManager->createEntity("OGRENGINE_NodeCube2", "cube.mesh", "General");
		cube->setMaterialName("doubleTexture_M");
		cube->setCastShadows(false);

		m_cube2 = m_sceneManager->getRootSceneNode()->createChildSceneNode();
		m_cube2->translate(Ogre::Vector3(-100, 250, -100));
        m_cube2->attachObject(cube);
	}
	// Cube3
	//----------------------------------
	{
        ::Ogre::Entity* cube = m_sceneManager->createEntity("OGRENGINE_NodeCube3", "cube.mesh", "General");
        cube->setMaterialName("doubleTexture_M");
		cube->setCastShadows(false);

		m_cube3 = m_sceneManager->getRootSceneNode()->createChildSceneNode();
		m_cube3->translate(Ogre::Vector3(-100, 250, 100));
        m_cube3->attachObject(cube);
	}
	// Cube4
	//----------------------------------
	{
        ::Ogre::Entity* cube = m_sceneManager->createEntity("OGRENGINE_NodeCube4", "cube.mesh", "General");
        cube->setMaterialName("doubleTexture_M");
		cube->setCastShadows(false);

		m_cube4 = m_sceneManager->getRootSceneNode()->createChildSceneNode();
		m_cube4->translate(Ogre::Vector3(100, 250, -100));
        m_cube4->attachObject(cube);
	}
	// Sphere1
	//----------------------------------
	{
        ::Ogre::Entity* sphere = m_sceneManager->createEntity("OGRENGINE_NodeSphere1", "sphere.mesh", "General");
        sphere->setMaterialName("doubleTexture_M");
		sphere->setCastShadows(false);

		m_sphere1 = m_sceneManager->getRootSceneNode()->createChildSceneNode();
		m_sphere1->translate(Ogre::Vector3(0, 500, 0));
        m_sphere1->attachObject(sphere);
	}
	// Plane
	//---------------------------------
	{
        Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
		Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 5000, 5000, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
		Ogre::Entity* ent = m_sceneManager->createEntity("GroundEntity", "ground");
		m_sceneManager->getRootSceneNode()->attachObject(ent);
        ent->setMaterialName("doubleTexture_M");
        ent->setCastShadows(false);
	}
}

void Application::destroyObject()
{
	// Light1
	//----------------------------------
	{
		::Ogre::MovableObject* mov = m_sceneManager->getRootSceneNode()->detachObject("OGRENGINE_Light1");
		m_sceneManager->destroyMovableObject(mov);
	}
	// Light2
	//----------------------------------
	{
		::Ogre::MovableObject* mov = m_sceneManager->getRootSceneNode()->detachObject("OGRENGINE_Light2");
		m_sceneManager->destroyMovableObject(mov);
	}
	// Cube1
	//----------------------------------
	{
		::Ogre::MovableObject* mov = m_cube1->detachObject("OGRENGINE_NodeCube1");
		m_sceneManager->getRootSceneNode()->removeChild(m_cube1);
		m_sceneManager->destroyMovableObject(mov);
	}
	// Cube2
	//----------------------------------
	{
		::Ogre::MovableObject* mov = m_cube2->detachObject("OGRENGINE_NodeCube2");
		m_sceneManager->getRootSceneNode()->removeChild(m_cube2);
		m_sceneManager->destroyMovableObject(mov);
	}
	// Cube3
	//----------------------------------
	{
		::Ogre::MovableObject* mov = m_cube3->detachObject("OGRENGINE_NodeCube3");
		m_sceneManager->getRootSceneNode()->removeChild(m_cube3);
		m_sceneManager->destroyMovableObject(mov);
	}
	// Cube4
	//----------------------------------
	{
		::Ogre::MovableObject* mov = m_cube4->detachObject("OGRENGINE_NodeCube4");
		m_sceneManager->getRootSceneNode()->removeChild(m_cube4);
		m_sceneManager->destroyMovableObject(mov);
	}
	// Sphere1
	//----------------------------------
	{
		::Ogre::MovableObject* mov = m_sphere1->detachObject("OGRENGINE_NodeSphere1");
		m_sceneManager->getRootSceneNode()->removeChild(m_sphere1);
		m_sceneManager->destroyMovableObject(mov);
	}
	// Plane
	//---------------------------------
	{
        /*::Ogre::MovableObject* mov = m_sceneManager->getRootSceneNode()->detachObject("GroundEntity");
        m_sceneManager->destroyMovableObject(mov);*/
	}
}


// ================================= Events =================================

bool Application::frameStarted(const Ogre::FrameEvent &evt)
{
	{
		m_cameraYawNode->yaw(Ogre::Radian(m_rotation.x));
		m_cameraPitchNode->pitch(Ogre::Radian(m_rotation.y));
		m_rotation.x = 0.f;
		m_rotation.y = 0.f;
		m_cameraNode->translate(m_cameraYawNode->getOrientation() * m_cameraPitchNode->getOrientation() * m_translation, Ogre::SceneNode::TS_LOCAL);

		float pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(m_cameraPitchNode->getOrientation().w)).valueDegrees());
		float pitchAngleSign = m_cameraPitchNode->getOrientation().x;

		if (pitchAngle > 90.0f)
		{
			if (pitchAngleSign > 0)
				m_cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), Ogre::Math::Sqrt(0.5f), 0, 0));
			else if (pitchAngleSign < 0)
				m_cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), -Ogre::Math::Sqrt(0.5f), 0, 0));
		}

		if (m_cameraNode->_getFullTransform().getTrans().y < 10.f)
		{
			Ogre::Vector3 pos = m_cameraNode->getPosition();
			pos.y  = 10.f;
			m_cameraNode->setPosition(pos);
		}

	}

	return true;
}

bool Application::frameEnded(const Ogre::FrameEvent &evt)
{
	return true;
}

bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	// Rotate the cube1
	//---------------------------------
	{
		m_cube1->yaw(Ogre::Degree(Ogre::Real(0.02)));
		m_cube1->pitch(Ogre::Degree(Ogre::Real(0.03)));
		m_cube1->roll(Ogre::Degree(Ogre::Real(0.01)));
	}
	// Rotate the cube2
	//---------------------------------
	{
		m_cube2->yaw(Ogre::Degree(Ogre::Real(0.03)));
		m_cube2->pitch(Ogre::Degree(Ogre::Real(0.01)));
		m_cube2->roll(Ogre::Degree(Ogre::Real(0.02)));
	}
	// Rotate the cube3
	//---------------------------------
	{
		m_cube3->yaw(Ogre::Degree(Ogre::Real(0.01)));
		m_cube3->pitch(Ogre::Degree(Ogre::Real(0.02)));
		m_cube3->roll(Ogre::Degree(Ogre::Real(0.03)));
	}
	// Rotate the cube4
	//---------------------------------
	{
		m_cube4->yaw(Ogre::Degree(Ogre::Real(0.03)));
		m_cube4->pitch(Ogre::Degree(Ogre::Real(0.02)));
		m_cube4->roll(Ogre::Degree(Ogre::Real(0.01)));
	}
	return true;
}
