//#include "boost/asio.hpp"
#include "decentralised_app.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:WinMainCRTStartup")
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
#endif

#ifdef __cplusplus
	extern "C" {
#endif

using namespace decentralised;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char *argv[])
#endif
{
	decentralised_app app;

	try
	{
		app.go();
	}
	catch (Ogre::Exception& e)
	{
		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(
				NULL,
				e.getFullDescription().c_str(),
				"Afff! Exception!",
				MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			std::cerr << "Afff! Exception! > " <<
				e.getFullDescription().c_str() << std::endl;
		#endif
	}

	return 0;
}

#ifdef __cplusplus
  }
#endif
