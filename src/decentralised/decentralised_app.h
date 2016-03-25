#ifndef __DECENTRALISED_APP_H_INCLUDED__
#define __DECENTRALISED_APP_H_INCLUDED__

#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include "dc-config.h"
//#include "manager_gui.h"
#include "manager_filesystem.h"
//#include "manager_core.h"
//#include "irrlicht_event_handler.h"
#include "context_application.h"
//#include <OgreRoot.h>
//#include <OgreLogManager.h>
#include <OgreD3D9RenderSystem.h>
#include <OgreD3D11RenderSystem.h>
#include <OgreGLRenderSystem.h>
#include "Ogre.h" 
#include <OgreRoot.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

using namespace decentralised::managers;
using namespace decentralised::context;

namespace decentralised
{
	class decentralised_app
		: public Ogre::WindowEventListener,
		  public Ogre::FrameListener,
		  public OIS::KeyListener,
		  public OIS::MouseListener
	{
	public:
		decentralised_app();
		~decentralised_app();

		void go();

	private:
		virtual bool keyPressed(const OIS::KeyEvent& ke);
		virtual bool keyReleased(const OIS::KeyEvent& ke);

		virtual bool mouseMoved(const OIS::MouseEvent& me);
		virtual bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id);
		virtual bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id);

		virtual void windowResized(Ogre::RenderWindow* rw);
		virtual void windowClosed(Ogre::RenderWindow* rw);

		Ogre::RenderWindow* window_;
		context_application context_;
	};
}

#endif