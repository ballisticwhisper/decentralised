#ifndef __MANAGER_FILESYSTEM_H_INCLUDED__
#define __MANAGER_FILESYSTEM_H_INCLUDED__

#include <irrlicht.h>
#include "dc-config.h"
#include <vector>
#include <map>

using namespace irr;

using namespace irr::core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

namespace decentralised
{
	namespace managers
	{
		class manager_filesystem
		{
		public:
			manager_filesystem();
			~manager_filesystem();

			std::map<std::wstring, std::wstring> loadConfig();
			std::map<std::wstring, std::wstring> loadSkin(std::wstring skinName);
			std::map<std::wstring, std::wstring> loadLanguage(std::wstring langCode);

		private:
			std::map<std::wstring, std::wstring> toMap(stringw path);

			IrrlichtDevice* dev_;
		};
	}
}

#endif