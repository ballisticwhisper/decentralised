#include "lang.h"

lang::lang()
{
	dev_ = irr::createDevice(irr::video::EDT_NULL);
}

lang::~lang()
{
	dev_->drop();
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
	irr::io::IReadFile* file = dev_->getFileSystem()->createMemoryReadFile(bytes, dwSize, "setup-lang.xml");
	irr::io::IXMLReader* xml = dev_->getFileSystem()->createXMLReader(file);

	if (!xml)
		return results;

	while (xml->read())
	{
		switch (xml->getNodeType())
		{
		case irr::io::EXN_ELEMENT:
		{
			stringw nodeName = stringw(xml->getNodeName());

			if (nodeName == L"lang")
				results[xml->getAttributeValueSafe(L"code")] = xml->getAttributeValueSafe(L"name");

			break;
		}
		}
	}
	xml->drop();
	file->drop();
	delete bytes;

	return results;
}

std::map<std::wstring, std::wstring> lang::loadLanguage(std::string langCode)
{
	std::map<std::wstring, std::wstring> results;

	unsigned long dwSize = 0;

	char *bytes = getLangFile(&dwSize);
	irr::io::IReadFile* file = dev_->getFileSystem()->createMemoryReadFile(bytes, dwSize, "setup-lang.xml");
	irr::io::IXMLReader* xml = dev_->getFileSystem()->createXMLReader(file);

	if (!xml)
		return results;

	stringw currentCode = L"";

	while (xml->read())
	{
		switch (xml->getNodeType())
		{
		case irr::io::EXN_ELEMENT:
		{
			stringw nodeName = stringw(xml->getNodeName());

			if (nodeName == L"lang")
				currentCode = xml->getAttributeValueSafe(L"code");

			if (nodeName == L"setting" && currentCode.equals_ignore_case(langCode.c_str()))
				results[xml->getAttributeValueSafe(L"name")] = xml->getAttributeValueSafe(L"value");

			break;
		}
		}
	}
	xml->drop();
	file->drop();
	delete bytes;

	return results;
}

