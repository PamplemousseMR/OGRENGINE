#include "stdafx.h"
#include "ViewportListener.h"

ViewportListener::ViewportListener(::Ogre::Camera* const _camera)
	: m_camera(_camera)
{
}


ViewportListener::~ViewportListener()
{
	m_camera = nullptr;
}

void ViewportListener::viewportDimensionsChanged(::Ogre::Viewport* _viewport)
{
	m_camera->setAspectRatio(Ogre::Real(_viewport->getActualWidth() / _viewport->getActualHeight()));
}
