#include "lang.h"

lang::lang()
{
}

lang::~lang()
{
}

char* lang::getLangFile(unsigned long *dwSize)
{
	char *bytes = NULL;

#ifdef WIN32

	HMODULE hModule = GetModuleHandle(NULL);
	HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_XML_LANG), L"XML");
	HGLOBAL hMemory = LoadResource(hModule, hResource);
	*dwSize = SizeofResource(hModule, hResource);
	LPVOID lpAddress = LockResource(hMemory);

	bytes = new char[*dwSize];
	memcpy(bytes, lpAddress, *dwSize);

	UnlockResource(hMemory);
	FreeResource(hMemory);
#endif

	return bytes;
}

std::map<std::wstring, std::wstring> lang::loadLanguageNames()
{
	std::map<std::wstring, std::wstring> results;

	unsigned long dwSize = 0;

	char *bytes = getLangFile(&dwSize);
	tinyxml2::XMLDocument doc;
	if(doc.Parse(bytes) != tinyxml2::XMLError::XML_SUCCESS)
		return results;

	tinyxml2::XMLNode* language = doc.FirstChildElement("languages")->FirstChild();
	while (language != NULL)
	{
		tinyxml2::XMLElement* elem = language->ToElement();

		std::string name(elem->Attribute("code"));
		std::wstring namew(name.begin(), name.end());

		std::string val(elem->Attribute("name"));
		std::wstring valw(val.begin(), val.end());

		results[namew] = valw;

		language = language->NextSibling();
	}

	delete bytes;
	return results;
}

std::map<std::wstring, std::wstring> lang::loadLanguage(std::string langCode)
{
	std::map<std::wstring, std::wstring> results;

	unsigned long dwSize = 0;

	char *bytes = getLangFile(&dwSize);
	tinyxml2::XMLDocument doc;
	if (doc.Parse(bytes) != tinyxml2::XMLError::XML_SUCCESS)
		return results;

	tinyxml2::XMLNode* language = doc.FirstChildElement("languages")->FirstChild();

	while (language != NULL)
	{
		tinyxml2::XMLElement* elem = language->ToElement();

		std::string name(elem->Attribute("code"));
		if (name == langCode)
		{
			tinyxml2::XMLNode* setting = language->FirstChildElement("setting");
			while (setting != NULL)
			{
				tinyxml2::XMLElement* settingElem = setting->ToElement();

				std::string name(settingElem->Attribute("name"));
				std::wstring namew(name.begin(), name.end());

				std::string val(settingElem->Attribute("value"));
				std::wstring valw(val.begin(), val.end());

				results[namew] = valw;

				setting = setting->NextSibling();
			}
			break;
		}

		language = language->NextSibling();
	}

	delete bytes;
	return results;
}

