#define _IRR_STATIC_LIB_

#include <irrlicht.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include "dc-config.h"
#include "manager_gui.h"
#include "manager_filesystem.h"
#include "manager_core.h"
#include "irrlicht_event_handler.h"
#include "context_application.h"

using namespace irr;
using namespace irr::core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace decentralised::managers;
using namespace decentralised::context;

#ifdef _WINDOWS
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#pragma comment(lib, "irrlicht.lib")
#endif

template <typename T>
std::wstring to_wstring(T val)
{
	std::wstringstream stream;
	stream << val;
	return stream.str();
}

int main()
{
	E_DRIVER_TYPE driverType = EDT_OPENGL;
#if defined ( _IRR_WINDOWS_ )
	driverType = EDT_DIRECT3D9;
#endif

	manager_filesystem* fileManager = new manager_filesystem();
	context_application context;

	context.config = fileManager->loadConfig();
	context.skin = fileManager->loadSkin(context.config[L"Skin"]);
	context.language = fileManager->loadLanguage(context.config[L"Language"]);

	context.device = createDevice(driverType, irr::core::dimension2d<u32>(800, 600));
	if (context.device == 0)
		return 1;

	context.device->setResizable(true);

	std::wstring title = std::wstring(APP_TITLE)
		.append(L" ")
		.append(to_wstring(Decentralised_VERSION_MAJOR))
		.append(L".")
		.append(to_wstring(Decentralised_VERSION_MINOR))
		.append(L"\n").c_str();

	context.device->setWindowCaption(title.c_str());

	context.gui_manager = new manager_gui(context.device, context.language, context.skin, context.config);
	context.gui_manager->Initialize();

	context.world_manager = new manager_world(context.device);

	IVideoDriver* driver = context.device->getVideoDriver();

	context.gui_manager->AddConsoleLine(SColor(255,255,255,255), APP_TITLE);

	irrlicht_event_handler receiver(context);

	context.network_manager = new manager_core();

	context.network_manager->setEventReceiver(&receiver);
	context.device->setEventReceiver(&receiver);

	context.network_manager->start();

	ISceneManager *scene = context.device->getSceneManager();

	while (context.device->run() && driver)
		if (!context.device->isWindowMinimized() || context.device->isWindowActive())
		{
			driver->beginScene(true, true, SColor(0, 0, 0, 0));

			scene->drawAll();
			context.gui_manager->DrawAll();

			driver->endScene();
		}

	context.network_manager->stop();

	delete context.world_manager;
	delete context.gui_manager;
	delete context.network_manager;
	delete fileManager;

	return 0;
}