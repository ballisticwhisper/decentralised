#define _IRR_STATIC_LIB_

#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include "dc-config.h"
#include <irrlicht.h>
#include "manager_gui.h"
#include "manager_filesystem.h"

using namespace irr;
using namespace irr::core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace decentralised::managers;

#ifdef _IRR_WINDOWS_
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
	std::map<std::wstring, std::wstring> config = fileManager->loadConfig();
	std::map<std::wstring, std::wstring> skin = fileManager->loadSkin(config[L"Skin"]);
	std::map<std::wstring, std::wstring> lang = fileManager->loadLanguage(config[L"Language"]);

	IrrlichtDevice * device = createDevice(driverType, irr::core::dimension2d<u32>(800, 600));
	if (device == 0)
		return 1;

	device->setResizable(true);

	std::wstring title = std::wstring(APP_TITLE)
		.append(L" ")
		.append(to_wstring(Decentralised_VERSION_MAJOR))
		.append(L".")
		.append(to_wstring(Decentralised_VERSION_MINOR))
		.append(L"\n").c_str();

	device->setWindowCaption(title.c_str());

	manager_gui* guiManager = new manager_gui(device, lang, skin, config);
	guiManager->Initialize();

	IVideoDriver* driver = device->getVideoDriver();

	while (device->run() && driver)
		if (!device->isWindowMinimized() || device->isWindowActive())
		{
			driver->beginScene(true, true, SColor(0, 0, 0, 0));

			guiManager->DrawAll();

			driver->endScene();
		}

	delete guiManager;
	delete fileManager;

	return 0;
}