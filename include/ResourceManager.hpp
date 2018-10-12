#pragma once

#include <filesystem>
#include <iostream>

#include <OGRE/OgreString.h>
#include <OGRE/OgreResourceManager.h>

class ResourceManager
{

public:

    ResourceManager(const std::filesystem::path&);
    ~ResourceManager();

    void load();
    void unload();

    void initialiseAll() const;
    void initialise(const ::Ogre::String&) const;
    void clearAll() const;
    void clear(const ::Ogre::String&) const;

private:

    class Listener : public ::Ogre::ResourceLoadingListener
    {

    private:

        virtual ::Ogre::DataStreamPtr resourceLoading(const ::Ogre::String&, const ::Ogre::String&, ::Ogre::Resource*)
        {
            return ::Ogre::DataStreamPtr();
        }

        virtual void resourceStreamOpened(const ::Ogre::String& _name, const ::Ogre::String& _group, ::Ogre::Resource*, ::Ogre::DataStreamPtr&)
        {
            std::cout << "[" << __FUNCTION__ << "] Load resource : " << _group << " " << _name << " " << std::endl;
        }

        virtual bool resourceCollision(::Ogre::Resource*, ::Ogre::ResourceManager*)
        {
            return true;
        }
    };

    Listener m_listener {};
    std::filesystem::path m_path {};

};

