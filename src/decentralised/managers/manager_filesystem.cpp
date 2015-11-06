#include "manager_filesystem.h"

namespace decentralised
{
	namespace managers
	{
		manager_filesystem::manager_filesystem(void)
		{
			dev_ = irr::createDevice(irr::video::EDT_NULL);
		}

		manager_filesystem::~manager_filesystem()
		{
			if (dev_)
			{
				dev_->closeDevice();
				dev_->drop();
			}
		}

		std::map<std::wstring, std::wstring> manager_filesystem::loadConfig()
		{
			stringw configPath = APP_DATA_DIR;
			configPath.append("configuration.xml");

			return toMap(configPath);
		}

		std::map<std::wstring, std::wstring> manager_filesystem::loadSkin(std::wstring skinName)
		{
			stringw skinPath = APP_DATA_DIR;
			skinPath.append("Skins/");
			skinPath.append(skinName.c_str());
			skinPath.append("/skin.xml");

			return toMap(skinPath);
		}

		std::map<std::wstring, std::wstring> manager_filesystem::loadLanguage(std::wstring langCode)
		{
			stringw langPath = APP_DATA_DIR;
			langPath.append(langCode.c_str());
			langPath.append(".xml");

			return toMap(langPath);
		}

		std::map<std::wstring, std::wstring> manager_filesystem::toMap(stringw path)
		{
			std::map<std::wstring, std::wstring> results;

			if (!dev_)
				return results;

			irr::io::IXMLReader* xml = dev_->getFileSystem()->createXMLReader(path);
			if (!xml)
				return results;

			while (xml->read())
			{
				switch (xml->getNodeType())
				{
				case irr::io::EXN_ELEMENT:
				{
					stringw nodeName = stringw(xml->getNodeName());

					if (nodeName == L"setting")
						results[xml->getAttributeValueSafe(L"name")] = xml->getAttributeValueSafe(L"value");

					break;
				}
				}
			}
			xml->drop();

			return results;
		}
	}
}