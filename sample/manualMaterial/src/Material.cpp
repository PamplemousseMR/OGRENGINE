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

    pass->setAmbient(::Ogre::ColourValue(0.2f, 0.2f, 0.2f, 1.0));
    pass->setDiffuse(::Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0));
    pass->setSpecular(::Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0));
    pass->setShininess(33);

    {
        Ogre::HighLevelGpuProgramPtr vertex = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->createProgram(m_name + "_VS", m_group, "glsl", Ogre::GpuProgramType::GPT_VERTEX_PROGRAM);
        vertex->setSource("#version 440 core\n"
                          "\n"
                          "layout(location = 0) in vec3 position;\n"
                          "layout(location = 2) in vec3 normal;\n"
                          "layout(location = 8) in vec2 uv0;\n"
                          "\n"
                          "uniform mat4 u_m4Model;\n"
                          "uniform mat4 u_m4View;\n"
                          "uniform mat4 u_m4Projection;\n"
                          "\n"
                          "uniform vec3 u_f3LightPos_Vs;\n"
                          "\n"
                          "out vec2 v_f2TextCor_Fs;\n"
                          "out vec3 v_f3NormalDir_Vs;\n"
                          "out vec3 v_f3CamDir_Vs;\n"
                          "out vec3 v_f3LightDir_Vs;\n"
                          "\n"
                          "void main()\n"
                          "{\n"
                          "    gl_Position = u_m4Projection * u_m4View * u_m4Model * vec4(position,1);\n"
                          "\n"
                          "    vec3 vertexPos_Vs = ( u_m4View * u_m4Model * vec4(position,1)).xyz;\n"
                          "    v_f3CamDir_Vs = vec3(0,0,0) - vertexPos_Vs;\n"
                          "\n"
                          "    v_f3LightDir_Vs = u_f3LightPos_Vs-vertexPos_Vs;\n"
                          "\n"
                          "    v_f3NormalDir_Vs = ( u_m4View * u_m4Model * vec4(normal,0)).xyz;\n"
                          "\n"
                          "    v_f2TextCor_Fs = uv0;\n"
                          "}\n");

        ::Ogre::GpuProgramParametersSharedPtr vertexParameters = vertex->getDefaultParameters();
        vertexParameters->setNamedAutoConstant("u_m4Model", ::Ogre::GpuProgramParameters::ACT_WORLD_MATRIX);
        vertexParameters->setNamedAutoConstant("u_m4View", ::Ogre::GpuProgramParameters::ACT_VIEW_MATRIX);
        vertexParameters->setNamedAutoConstant("u_m4Projection", ::Ogre::GpuProgramParameters::ACT_PROJECTION_MATRIX);

        vertexParameters->setNamedAutoConstant("u_f3LightPos_Vs", ::Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_VIEW_SPACE, 0);

        vertex->load();

        pass->setVertexProgram(vertex->getName());
    }

    {
        Ogre::HighLevelGpuProgramPtr fragment = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->createProgram(m_name + "_FS", m_group, "glsl", Ogre::GpuProgramType::GPT_FRAGMENT_PROGRAM);
        fragment->setSource("#version 440 core\n"
                            "\n"
                            "in vec2 v_f2TextCor_Fs;\n"
                            "in vec3 v_f3NormalDir_Vs;\n"
                            "in vec3 v_f3CamDir_Vs;\n"
                            "in vec3 v_f3LightDir_Vs;\n"
                            "\n"
                            "uniform sampler2D u_tAmbient;\n"
                            "uniform sampler2D u_tDiffuse;\n"
                            "uniform sampler2D u_tNormal;\n"
                            "uniform sampler2D u_tSpecular;\n"
                            "\n"
                            "uniform vec3 u_f3AmbientCol;\n"
                            "uniform vec3 u_f3DiffuseCol;\n"
                            "uniform vec3 u_f3SpecularCol;\n"
                            "uniform float u_fSpecularExponent;\n"
                            "\n"
                            "uniform vec3 u_f3LightAmbientCol;\n"
                            "uniform vec3 u_f3LightDiffuseCol;\n"
                            "uniform vec3 u_f3LightSpecularCol;\n"
                            "\n"
                            "layout(location = 0) out vec4 v_f4OutCol;\n"
                            "\n"
                            "void main()\n"
                            "{\n"
                            "    vec3 f3NormalDir_VsN = normalize(v_f3NormalDir_Vs);\n"
                            "    vec3 f3LightDir_VsN = normalize(v_f3LightDir_Vs);\n"
                            "    float cosTheta = clamp( dot(f3NormalDir_VsN, f3LightDir_VsN), 0, 1);\n"
                            "    vec3 f3CamDir_VsN = normalize(v_f3CamDir_Vs);\n"
                            "    vec3 f3CamReflectDir_VsN = reflect(-f3LightDir_VsN,f3NormalDir_VsN);\n"
                            "    float cosAlpha = clamp(dot(f3CamDir_VsN, f3CamReflectDir_VsN), 0, 1);\n"
                            "\n"
                            "    // ambient\n"
                            "    vec3 f3AmbientCol = texture(u_tAmbient, vec2(v_f2TextCor_Fs.x, 1 - v_f2TextCor_Fs.y)).rgb;\n"
                            "    f3AmbientCol = f3AmbientCol * u_f3LightAmbientCol;\n"
                            "\n"
                            "    // diffuse\n"
                            "    vec4 f4DiffuseCol = texture(u_tDiffuse, vec2(v_f2TextCor_Fs.x, 1 - v_f2TextCor_Fs.y));\n"
                            "    vec3 f3DiffuseCol = f4DiffuseCol.rgb;\n"
                            "    float fAlpha = f4DiffuseCol.a;\n"
                            "    f3DiffuseCol = f3DiffuseCol * u_f3LightDiffuseCol * cosTheta;\n"
                            "\n"
                            "    // specular\n"
                            "    vec3 f3SpecularCol = texture(u_tSpecular, vec2(v_f2TextCor_Fs.x, 1 - v_f2TextCor_Fs.y)).rgb;\n"
                            "    f3SpecularCol = f3SpecularCol * u_f3LightSpecularCol * pow(cosAlpha, u_fSpecularExponent);\n"
                            "\n"
                            "    v_f4OutCol = vec4(u_f3AmbientCol * f3AmbientCol + u_f3DiffuseCol * f3DiffuseCol + u_f3SpecularCol * f3SpecularCol ,fAlpha);\n"
                            "}\n"
                            "\n");

        ::Ogre::GpuProgramParametersSharedPtr fragmentParameters = fragment->getDefaultParameters();
        fragmentParameters->setNamedAutoConstant("u_f3AmbientCol", ::Ogre::GpuProgramParameters::ACT_SURFACE_AMBIENT_COLOUR);
        fragmentParameters->setNamedAutoConstant("u_f3DiffuseCol", ::Ogre::GpuProgramParameters::ACT_SURFACE_DIFFUSE_COLOUR);
        fragmentParameters->setNamedAutoConstant("u_f3SpecularCol", ::Ogre::GpuProgramParameters::ACT_SURFACE_SPECULAR_COLOUR);
        fragmentParameters->setNamedAutoConstant("u_fSpecularExponent", ::Ogre::GpuProgramParameters::ACT_SURFACE_SHININESS);

        fragmentParameters->setNamedAutoConstant("u_f3LightAmbientCol", ::Ogre::GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR, 0);
        fragmentParameters->setNamedAutoConstant("u_f3LightDiffuseCol", ::Ogre::GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR, 0);
        fragmentParameters->setNamedAutoConstant("u_f3LightSpecularCol", ::Ogre::GpuProgramParameters::ACT_LIGHT_SPECULAR_COLOUR, 0);

        fragmentParameters->setNamedConstant("u_tAmbient", 0);
        fragmentParameters->setNamedConstant("u_tDiffuse", 1);
        fragmentParameters->setNamedConstant("u_tNormal", 2);
        fragmentParameters->setNamedConstant("u_tSpecular",3);

        fragment->load();

        pass->setFragmentProgram(fragment->getName());
    }

    ::Ogre::TextureUnitState* pTex1 = pass->createTextureUnitState();
    pTex1->setTextureName("rock_diffuse.jpg");
    pTex1->setTextureFiltering(::Ogre::FilterOptions::FO_LINEAR, ::Ogre::FilterOptions::FO_LINEAR, ::Ogre::FilterOptions::FO_LINEAR);
    ::Ogre::TextureUnitState* pTex2 = pass->createTextureUnitState();
    pTex2->setTextureName("rock_diffuse.jpg");
    pTex2->setTextureFiltering(::Ogre::FilterOptions::FO_LINEAR, ::Ogre::FilterOptions::FO_LINEAR, ::Ogre::FilterOptions::FO_LINEAR);
    ::Ogre::TextureUnitState* pTex3 = pass->createTextureUnitState();
    pTex3->setTextureName("rock_normal.jpg");
    pTex3->setTextureFiltering(::Ogre::FilterOptions::FO_LINEAR, ::Ogre::FilterOptions::FO_LINEAR, ::Ogre::FilterOptions::FO_LINEAR);
    ::Ogre::TextureUnitState* pTex4 = pass->createTextureUnitState();
    pTex4->setTextureName("rock_specular.png");
    pTex4->setTextureFiltering(::Ogre::FilterOptions::FO_LINEAR, ::Ogre::FilterOptions::FO_LINEAR, ::Ogre::FilterOptions::FO_LINEAR);
}

void Material::unload()
{
    std::cout << "[" << __FUNCTION__ << "] " << m_name << "..." << std::endl;
    ::Ogre::MaterialManager::getSingleton().remove(m_name, m_group);
    m_material.reset();
}
