#define _IRR_STATIC_LIB_

#include <stdio.h>
#include <string>
#include <iostream>
#include "dc-config.h"
#include <irrlicht.h>

using namespace irr;
using namespace irr::core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#pragma comment(lib, "irrlicht.lib")
#endif

int main()
{
	E_DRIVER_TYPE driverType = EDT_OPENGL;
#if defined ( _IRR_WINDOWS_ )
	driverType = EDT_DIRECT3D9;
#endif

	IrrlichtDevice * device = createDevice(driverType, irr::core::dimension2d<u32>(800, 600));

	printf(std::string("Version ")
		.append(to_string(Decentralised_VERSION_MAJOR))
		.append(".")
		.append(to_string(Decentralised_VERSION_MINOR))
		.append("\n").c_str());

	printf("Gotta start sumwhere\n");

	std::string input = "";
	getline(std::cin, input);

	return 0;
}