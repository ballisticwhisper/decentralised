#include "manager_filesystem.h"

namespace decentralised
{
	namespace managers
	{
		manager_filesystem::manager_filesystem(void)
		{
		}

		manager_filesystem::~manager_filesystem()
		{
		}

		std::map<std::wstring, std::wstring> manager_filesystem::loadConfig()
		{
			std::wstring configPath = APP_DATA_DIR;
			configPath.append(L"configuration.xml");

			return toMap(configPath);
		}

		std::map<std::wstring, std::wstring> manager_filesystem::loadSkin(std::wstring skinName)
		{
			std::wstring skinPath = APP_DATA_DIR;
			skinPath.append(L"Skins/");
			skinPath.append(skinName.c_str());
			skinPath.append(L"/skin.xml");

			return toMap(skinPath);
		}

		std::map<std::wstring, std::wstring> manager_filesystem::loadLanguage(std::wstring langCode)
		{
			std::wstring langPath = APP_DATA_DIR;
			langPath.append(L"Lang/");
			langPath.append(langCode.c_str());
			langPath.append(L".xml");

			return toMap(langPath);
		}

		std::map<std::wstring, std::wstring> manager_filesystem::toMap(std::wstring path)
		{
			std::map<std::wstring, std::wstring> results;

			tinyxml2::XMLDocument doc;
			if (doc.LoadFile(std::string(path.begin(), path.end()).c_str()) != 0)
				return results;

			tinyxml2::XMLNode* elem = doc.FirstChild()->NextSibling()->FirstChildElement("setting");
			auto test = elem->Value();

			while (elem != NULL)
			{
				tinyxml2::XMLElement* settingElem = elem->ToElement();

				std::string name(settingElem->Attribute("name"));
				std::wstring namew(name.begin(), name.end());

				std::string val(settingElem->Attribute("value"));
				std::wstring valw(val.begin(), val.end());

				results[namew] = valw;

				elem = elem->NextSibling();
			}
			return results;
		}
	}
}