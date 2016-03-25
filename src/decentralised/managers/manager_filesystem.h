#ifndef __MANAGER_FILESYSTEM_H_INCLUDED__
#define __MANAGER_FILESYSTEM_H_INCLUDED__

#include "dc-config.h"
#include <vector>
#include <map>
#include <tinyxml2.h>

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
			std::map<std::wstring, std::wstring> toMap(std::wstring path);
		};
	}
}

#endif