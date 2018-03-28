#include "stdafx.h"
#include "Material.h"

#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreHighLevelGpuProgram.h>
#include <OgreHighLevelGpuProgramManager.h>
#include <OgreTextureManager.h>

#include <iostream>

Material::Material(const std::string& _name)
	: m_name(_name)
{
}

Material::~Material()
{
}

void Material::start()
{
	std::cout << "[" << __FUNCTION__ << "] " << "Start : " << m_name << std::endl;
	m_material = ::Ogre::MaterialManager::getSingleton().create(m_name, ::Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	::Ogre::Technique* technique = m_material->getTechnique(0);

	::Ogre::Pass* pass1 = technique->getPass(0);

	::Ogre::Pass* pass2 = technique->createPass();
	pass2->setSceneBlending(::Ogre::SceneBlendType::SBT_MODULATE);

	::Ogre::TexturePtr checker = ::Ogre::TextureManager::getSingleton().getByName("checker.png");
	
	{
		Ogre::HighLevelGpuProgramPtr vertex = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->createProgram(m_name + "_VS", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "glsl", Ogre::GpuProgramType::GPT_VERTEX_PROGRAM);
		vertex->setSource("#version 330\n"
			"layout(location = 0)  in vec4 position;\n"	// magic name
			"in vec2 uv0;\n"							// magic name
			"uniform mat4 u_worldViewProj;\n"
			"out vec2 o_textCoord;\n"
			"void main()\n"
			"{\n"
			"	gl_Position = u_worldViewProj * position;\n"
			"	o_textCoord = uv0;\n"
			"}\n");

		::Ogre::GpuProgramParametersSharedPtr vertexParameters = vertex->getDefaultParameters();
		vertexParameters->setNamedAutoConstant("u_worldViewProj", ::Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);

		vertex->load();

		pass1->setVertexProgram(vertex->getName());
		pass2->setVertexProgram(vertex->getName());
	}

	{
		Ogre::HighLevelGpuProgramPtr fragment = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->createProgram(m_name + "_FS", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "glsl", Ogre::GpuProgramType::GPT_FRAGMENT_PROGRAM);
		fragment->setSource("#version 330\n"
			"out vec4 fragColor;\n"
			"uniform sampler2D texture1;\n"
			"in vec2 o_textCoord;\n"
			"void main()\n"
			"{\n"
			"	fragColor = texture2D(texture1, o_textCoord);\n"
			"	fragColor.a = 0.0;\n"
			"}\n");

		fragment->load();

		pass1->setFragmentProgram(fragment->getName());

		::Ogre::TextureUnitState* pTex1 = pass1->createTextureUnitState();
		pTex1->setTextureName("checker.png");

		pass2->setFragmentProgram(fragment->getName());

		::Ogre::GpuProgramParametersSharedPtr pass2fragmentParameters = pass2->getFragmentProgramParameters();

		::Ogre::TextureUnitState* pTex2 = pass2->createTextureUnitState();
		pTex2->setTextureName("MtlPlat2.jpg");
	
	}
}

void Material::stop()
{
	std::cout << "[" << __FUNCTION__ << "] " << "Stop : " << m_name << std::endl;
	::Ogre::MaterialManager::getSingleton().remove(m_name, ::Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	m_material.reset();
}