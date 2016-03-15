#ifndef __LANG_H_INCLUDED__
#define __LANG_H_INCLUDED__

#include <vector>
#include <map>
#include "dc-config.h"
#include <irrlicht.h>
#include <windows.h>
#include "../resource.h"

using namespace irr;

using namespace irr::core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class lang
{
public:
	lang();
	~lang();

	std::map<std::wstring, std::wstring> loadLanguage(std::string langCode);

	std::map<std::wstring, std::wstring> loadLanguageNames();
private:

	char* getLangFile(unsigned long *dwSize);

	IrrlichtDevice* dev_;
};


#endif