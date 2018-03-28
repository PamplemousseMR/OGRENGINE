#include "stdafx.h"
#include "Compositor.h"

#include <OgreCompositorManager.h>
#include <OgreCompositor.h>
#include <OgreCompositionTargetPass.h>
#include <OgreCompositionPass.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreHighLevelGpuProgram.h>
#include <OgreHighLevelGpuProgramManager.h>

#include <iostream>

Compositor::Compositor(const std::string& _name, ::Ogre::Viewport* const _viewport)
	:	m_name(_name),
		m_viewport(_viewport),
		m_compositor(nullptr)
{
}


Compositor::~Compositor()
{
}

void Compositor::start()
{
	std::cout << "[" << __FUNCTION__ << "] " << "Start : " << m_name << std::endl;

	createMaterial();

	::Ogre::CompositorManager& compositorManager = ::Ogre::CompositorManager::getSingleton();
	::Ogre::CompositorPtr compositor = compositorManager.create(m_name, ::Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);

	::Ogre::CompositionTechnique* technique = compositor->createTechnique();

	::Ogre::CompositionTechnique::TextureDefinition* renderTarget0 = technique->createTextureDefinition(m_name+"_RENDERTARGET0");

	{
		::Ogre::CompositionTargetPass* pass0 = technique->createTargetPass();

		pass0->setOutputName(renderTarget0->name);
		pass0->setInputMode(::Ogre::CompositionTargetPass::InputMode::IM_PREVIOUS);
	}

	{
		::Ogre::CompositionTargetPass* output = technique->getOutputTargetPass();
		output->setInputMode(::Ogre::CompositionTargetPass::InputMode::IM_NONE);

		::Ogre::CompositionPass* pass = output->createPass();
		pass->setMaterial(m_material);
	}

	compositorManager.addCompositor(m_viewport, m_name, true);
}

void Compositor::stop()
{
	std::cout << "[" << __FUNCTION__ << "] " << "Stop : " << m_name << std::endl;
	::Ogre::MaterialManager::getSingleton().remove(m_name + "_Material", ::Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	m_material.reset();
}

void Compositor::createMaterial()
{
	m_material = ::Ogre::MaterialManager::getSingleton().create(m_name + "_Material", ::Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	::Ogre::Technique* technique = m_material->getTechnique(0);

	::Ogre::Pass* pass = technique->getPass(0);

	{
		Ogre::HighLevelGpuProgramPtr vertex = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->createProgram(m_name + "_VS", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "glsl", Ogre::GpuProgramType::GPT_VERTEX_PROGRAM);
		vertex->setSource("#version 150\n"
			"in vec4 vertex;\n"
			"void main()\n"
			"{\n"
			"	vec2 inPos = sign(vertex.xy);\n"
			"	gl_Position = vec4(inPos.xy, 0.0, 1.0);\n"
			"}\n");

		vertex->load();

		pass->setVertexProgram(vertex->getName());
	}

	{
		Ogre::HighLevelGpuProgramPtr fragment = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->createProgram(m_name + "_FS", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "glsl", Ogre::GpuProgramType::GPT_FRAGMENT_PROGRAM);
		fragment->setSource("#version 150\n"
			"out vec4 fragColor;\n"
			"void main()\n"
			"{\n"
			"	fragColor = vec4(1.f, 0.f, 0.f, 0.f);\n"
			"}\n");

		fragment->load();

		pass->setFragmentProgram(fragment->getName());
	}
}