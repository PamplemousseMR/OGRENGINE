#pragma once

#include <OgreViewport.h>
#include <OgreCompositorInstance.h>

#include <string>

class Compositor
{

public:

	Compositor(const std::string&, ::Ogre::Viewport* const);
	~Compositor();
	void start();
	void stop();

private:

	const std::string m_name;
	::Ogre::Viewport* const m_viewport;
	::Ogre::CompositorInstance* m_compositor;
	::Ogre::MaterialPtr m_material;

	void createMaterial();

};

