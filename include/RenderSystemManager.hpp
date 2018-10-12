#pragma once

#include <OGRE/OgreRenderSystem.h>

class RenderSystemManager
{

public:

    enum RENDERSYSTEMMANAGER_TYPE
    {
        DIRECTX9,
        DIRECTX11,
        OPENGL,
        OPENGL3
    };

    enum RENDERSYSTEMMANAGER_FSAA : int
    {
        FSAA_0 = 0,
        FSAA_2 = 2,
        FSAA_4 = 4,
        FSAA_8 = 8,
        FSAA_16 = 16
    };

    enum RENDERSYSTEMMANAGER_DEPTH : int
    {
        DEPTH_32 = 32
    };

    enum RENDERSYSTEMMANAGER_FREQUENCY : int
    {
        FREQUENCY_60 = 60
    };

    enum RENDERSYSTEMMANAGER_RTT
    {
        RTT_FBO
    };

    enum RENDERSYSTEMMANAGER_INTERVAL : int
    {
        INTERVAL_1 = 1,
        INTERVAL_2 = 2,
        INTERVAL_3 = 3,
        INTERVAL_4 = 4
    };

    enum RENDERSYSTEM_MODE
    {
        MODE_640x480,
        MODE_720x480,
        MODE_720x576,
        MODE_800x600,
        MODE_1024x768,
        MODE_1152x864,
        MODE_1176x664,
        MODE_1280x720,
        MODE_1280x768,
        MODE_1280x800,
        MODE_1280x960,
        MODE_1280x024,
        MODE_1360x768,
        MODE_1366x768,
        MODE_1440x480,
        MODE_1440x900,
        MODE_1600x900,
        MODE_1600x024,
        MODE_1680x050,
        MODE_1768x992,
        MODE_1920x080
    };

public:

    RenderSystemManager();
    ~RenderSystemManager();

    void load(RENDERSYSTEMMANAGER_TYPE _type);
    void unload();

    void setColourDepth(RENDERSYSTEMMANAGER_DEPTH);
    void setDisplayFrequency(RENDERSYSTEMMANAGER_FREQUENCY);
    void setFSAA(RENDERSYSTEMMANAGER_FSAA);
    void setFullScreen(bool);
    void setRTTPreferredMode(RENDERSYSTEMMANAGER_RTT);
    void setVSync(bool);
    void setVSyncInterval(RENDERSYSTEMMANAGER_INTERVAL);
    void setVideoMode(RENDERSYSTEM_MODE);
    void setsRGBGammaConversion(bool);

private:

    ::Ogre::RenderSystem* m_renderSystem {nullptr};

};

