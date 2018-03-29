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
		pass->setType(::Ogre::CompositionPass::PassType::PT_RENDERQUAD);
		pass->setMaterial(m_material);
		pass->setInput(0, renderTarget0->name);
	}

	compositorManager.addCompositor(m_viewport, m_name);
	compositorManager.setCompositorEnabled(m_viewport, m_name, true);
}

void Compositor::stop()
{
	std::cout << "[" << __FUNCTION__ << "] " << "Stop : " << m_name << std::endl;
	::Ogre::CompositorManager::getSingleton().removeCompositor(m_viewport, m_name);
	::Ogre::MaterialManager::getSingleton().remove(m_name + "_Material", ::Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	m_material.reset();
}

void Compositor::createMaterial()
{
	m_material = ::Ogre::MaterialManager::getSingleton().create(m_name + "_Material", ::Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	::Ogre::Technique* technique = m_material->getTechnique(0);

	::Ogre::Pass* pass = technique->getPass(0);
	pass->setDepthCheckEnabled(false);
	pass->setPolygonModeOverrideable(false);

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

		pass->setVertexProgram(vertex->getName());
	}

	{
		Ogre::HighLevelGpuProgramPtr fragment = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->createProgram(m_name + "_FS", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "glsl", Ogre::GpuProgramType::GPT_FRAGMENT_PROGRAM);
		fragment->setSource("#version 330\n"
			"out vec4 fragColor;\n"
			"uniform sampler2D RT;\n"
			"in vec2 o_textCoord;\n"
			"void main()\n"
			"{\n"
			"	vec3 greyscale = vec3(dot(texture2D(RT, o_textCoord).rgb, vec3(0.3, 0.59, 0.11)));\n"
			"	fragColor = vec4(greyscale, 1.0);\n"
			"}\n");

		fragment->load();

		::Ogre::TextureUnitState* pTex = pass->createTextureUnitState();
		pTex->setTextureFiltering(::Ogre::TextureFilterOptions::TFO_TRILINEAR);

		pass->setFragmentProgram(fragment->getName());
	}
}