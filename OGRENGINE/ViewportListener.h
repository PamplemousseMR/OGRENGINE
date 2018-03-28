#pragma once

#include <OgreViewport.h>
#include <OgreCamera.h>

class ViewportListener : public ::Ogre::Viewport::Listener
{

public:

	ViewportListener(::Ogre::Camera* const);
	~ViewportListener();

	virtual void viewportDimensionsChanged(::Ogre::Viewport*) override;

private:

	::Ogre::Camera* m_camera;

};

