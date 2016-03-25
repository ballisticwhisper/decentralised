#ifndef __LANG_H_INCLUDED__
#define __LANG_H_INCLUDED__

#include <vector>
#include <map>
#include "dc-config.h"
#include <windows.h>
#include "../resource.h"
#include <tinyxml2.h>

class lang
{
public:
	lang();
	~lang();

	std::map<std::wstring, std::wstring> loadLanguage(std::string langCode);

	std::map<std::wstring, std::wstring> loadLanguageNames();
private:

	char* getLangFile(unsigned long *dwSize);
};


#endif