#include "Material.hpp"

#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreTechnique.h>
#include <OGRE/OgrePass.h>
#include <OGRE/OgreHighLevelGpuProgram.h>
#include <OGRE/OgreHighLevelGpuProgramManager.h>
#include <OGRE/OgreTextureManager.h>

#include <iostream>

Material::Material(const std::string& _name, const std::string& _group) :
    m_name(_name),
    m_group(_group)
{
}

Material::~Material()
{
}

void Material::load()
{
    std::cout << "[" << __FUNCTION__ << "] " << m_name << "..." << std::endl;

    m_material = ::Ogre::MaterialManager::getSingleton().create(m_name, m_group);
    ::Ogre::Technique* technique = m_material->getTechnique(0);
    ::Ogre::Pass* pass = technique->getPass(0);

    ::Ogre::TexturePtr checkerboard = ::Ogre::TextureManager::getSingleton().getByName("checkerboard.png", "ManualMaterial");

    {
        Ogre::HighLevelGpuProgramPtr vertex = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->createProgram(m_name + "_VS", m_group, "glsl", Ogre::GpuProgramType::GPT_VERTEX_PROGRAM);
        vertex->setSource("#version 330\n"
            "layout(location = 0)  in vec4 position;\n"	// magic name
            "in vec2 uv0;\n"							// magic name
            ""
            "uniform mat4 u_worldViewProj;\n"
            ""
            "out vec2 o_textCoord;\n"
            ""
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
        Ogre::HighLevelGpuProgramPtr fragment = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->createProgram(m_name + "_FS", m_group, "glsl", Ogre::GpuProgramType::GPT_FRAGMENT_PROGRAM);
        fragment->setSource("#version 330\n"
            "uniform sampler2D u_texture_1;"
            "uniform sampler2D u_texture_2;"
            ""
            "in vec2 o_textCoord;"
            ""
            "out vec4 fragColor;"
            ""
            "void main()"
            "{"
            "    fragColor = texture2D(u_texture_1, o_textCoord) * texture2D(u_texture_2, o_textCoord);"
            "}\n");

        fragment->load();

        pass->setFragmentProgram(fragment->getName());

        /*::Ogre::TextureUnitState* pTex1 = pass->createTextureUnitState();
        pTex1->setTexture(checkerboard);
        ::Ogre::TextureUnitState* pTex2 = pass->createTextureUnitState();
        pTex2->setTextureName("stainlessSteel.jpg");*/
    }
}

void Material::unload()
{
    std::cout << "[" << __FUNCTION__ << "] " << m_name << "..." << std::endl;
    ::Ogre::MaterialManager::getSingleton().remove(m_name, m_group);
    m_material.reset();
}
