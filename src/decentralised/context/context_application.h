#ifndef __CONTEXT_APPLICATION_H_INCLUDED__
#define __CONTEXT_APPLICATION_H_INCLUDED__

#include <irrlicht.h>
#include "manager_gui.h"
#include "CSceneManager.h"
#include "manager_network.h"

using namespace irr;
using namespace decentralised::managers;
using namespace decentralised::core;

namespace decentralised
{
	namespace context
	{
		struct context_application
		{
			IrrlichtDevice *	  device;
			manager_gui*		  gui_manager;
			manager_network*	  network_manager;
			CSceneManager*		  scene;
			std::map<std::wstring, std::wstring> skin;
			std::map<std::wstring, std::wstring> language;
			std::map<std::wstring, std::wstring> config;
		};
	}
}
#endif