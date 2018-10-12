#pragma once

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

public:

    RenderSystemManager();
    ~RenderSystemManager();

    void load(RENDERSYSTEMMANAGER_TYPE _type);
    void unload();

};

