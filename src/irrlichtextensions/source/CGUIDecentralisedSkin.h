#ifndef __C_GUI_DECENTRALISED_SKIN_H_INCLUDED__
#define __C_GUI_DECENTRALISED_SKIN_H_INCLUDED__

#include <map>
#include <vector>
#include "irrlicht.h"
#include "CGUISkin.h"
using namespace irr;
using namespace core;
using namespace gui;
using namespace video;

class CGUIDecentralisedSkin : public irr::gui::CGUISkin
{
public:
	CGUIDecentralisedSkin(IVideoDriver* driver, std::map<std::wstring, std::wstring> &skin);
	~CGUIDecentralisedSkin();

	virtual void draw3DToolBar(IGUIElement* element,
		const core::rect<s32>& rect,
		const core::rect<s32>* clip = 0);

	virtual SColor getColor(EGUI_DEFAULT_COLOR color) const;
	virtual SColor getColorFromSkin(std::wstring name) const;

private:
	SColor toColor(std::wstring csv) const;

	IVideoDriver* driver_;
	std::map<std::wstring, std::wstring> &skin_;
};

#endif