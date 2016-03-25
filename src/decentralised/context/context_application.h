#ifndef __CONTEXT_APPLICATION_H_INCLUDED__
#define __CONTEXT_APPLICATION_H_INCLUDED__

#include <OgreRoot.h>
#include "manager_gui.h"
#include "manager_core.h"
#include "manager_world.h"
#include "core/utility/elliptic_curve_key.hpp"

//using namespace decentralised::managers;
using namespace decentralised::core;

namespace decentralised
{
	namespace context
	{
		struct context_application
		{
			Ogre::Root*								ogre_root;
			//manager_gui*							gui_manager;
			//manager_core*							network_manager;
			//manager_world*							world_manager;
			//CSceneManager*							scene;
			std::map<std::wstring, std::wstring>	skin;
			std::map<std::wstring, std::wstring>	language;
			std::map<std::wstring, std::wstring>	config;
			elliptic_curve_key						current_key_pair;
		};
	}
}
#endif