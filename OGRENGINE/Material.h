#pragma once

#include <OgreString.h>
#include <OgreMaterial.h>

class Material
{

public:

	Material(const std::string&);
	~Material();
	void start();
	void stop();

private:

	::Ogre::String m_name;
	::Ogre::MaterialPtr m_material;

};