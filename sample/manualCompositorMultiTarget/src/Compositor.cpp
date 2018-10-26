#include "Compositor.hpp"

#include <OGRE/OgreCompositorManager.h>
#include <OGRE/OgreCompositionTargetPass.h>
#include <OGRE/OgreCompositionPass.h>
#include <OGRE/OgreResource.h>
#include <OGRE/OgreCompositor.h>
#include <OGRE/OgreMaterialManager.h>

#include <iostream>

Compositor::Compositor(const std::string& _name, const std::string& _group) :
    m_name(_name),
    m_group(_group)
{
}

Compositor::~Compositor()
{
}

void Compositor::load()
{
    std::cout << "[" << __FUNCTION__ << "] " << m_name << "..." << std::endl;
    m_compositor = ::Ogre::CompositorManager::getSingleton().create(m_name, m_group);

    ::Ogre::CompositionTechnique* technique = m_compositor->createTechnique();

    std::string targetName = m_name + "_target";
    ::Ogre::CompositionTechnique::TextureDefinition* target = technique->createTextureDefinition(targetName);
    target->scope = ::Ogre::CompositionTechnique::TextureScope::TS_CHAIN;
    target->formatList.push_back(::Ogre::PixelFormat::PF_A8R8G8B8);
    target->heightFactor = 1;
    target->widthFactor  = 1;

    std::string multipleTargetName = m_name + "_multipleTarget";
    ::Ogre::CompositionTechnique::TextureDefinition* multipleTarget = technique->createTextureDefinition(multipleTargetName);
    multipleTarget->scope = ::Ogre::CompositionTechnique::TextureScope::TS_CHAIN;
    multipleTarget->formatList.push_back(::Ogre::PixelFormat::PF_A8R8G8B8);
    multipleTarget->formatList.push_back(::Ogre::PixelFormat::PF_A8R8G8B8);
    multipleTarget->formatList.push_back(::Ogre::PixelFormat::PF_A8R8G8B8);
    multipleTarget->heightFactor = 1;
    multipleTarget->widthFactor  = 1;


    {
        ::Ogre::CompositionTargetPass* targetPass = technique->createTargetPass();
        targetPass->setOutputName(targetName);
        targetPass->setInputMode(Ogre::CompositionTargetPass::InputMode::IM_PREVIOUS);
    }

    {
        ::Ogre::CompositionTargetPass* targetPass = technique->createTargetPass();
        targetPass->setOutputName(multipleTargetName);
        targetPass->setInputMode(::Ogre::CompositionTargetPass::InputMode::IM_NONE);

        ::Ogre::CompositionPass* pass = targetPass->createPass();
        pass->setType(::Ogre::CompositionPass::PassType::PT_RENDERQUAD);
        ::Ogre::MaterialPtr material = ::Ogre::MaterialManager::getSingleton().getByName("manual_compositor_multi_target_draw_M");
        pass->setMaterial(material);
        pass->setInput(0, targetName);
    }

    {
        ::Ogre::CompositionTargetPass* targetPass = technique->getOutputTargetPass();
        targetPass->setInputMode(::Ogre::CompositionTargetPass::InputMode::IM_NONE);

        ::Ogre::CompositionPass* pass = targetPass->createPass();
        pass->setType(::Ogre::CompositionPass::PassType::PT_RENDERQUAD);
        ::Ogre::MaterialPtr material = ::Ogre::MaterialManager::getSingleton().getByName("manual_compositor_multi_target_merge_M");
        pass->setMaterial(material);
        pass->setInput(0, multipleTargetName, 0);
        pass->setInput(1, multipleTargetName, 1);
        pass->setInput(2, multipleTargetName, 2);
        pass->setInput(3, targetName);
    }
}

void Compositor::unload()
{
    std::cout << "[" << __FUNCTION__ << "] " << m_name << "..." << std::endl;
    ::Ogre::CompositorManager::getSingleton().remove(m_compositor);
    m_compositor.reset();
}
