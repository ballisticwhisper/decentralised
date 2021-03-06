#ifndef __C_GUI_DECENTRALISED_MENU_BAR_H_INCLUDED__
#define __C_GUI_DECENTRALISED_MENU_BAR_H_INCLUDED__

#include <map>
#include <vector>
#include "irrlicht.h"
#include "CGUIMenu.h"
#include "CGUIDecentralisedContextMenu.h"
#include "CGUIDecentralisedSkin.h"

using namespace irr;
using namespace core;
using namespace gui;
using namespace video;

class CGUIDecentralisedMenuBar : public CGUIDecentralisedContextMenu
{
public:
	CGUIDecentralisedMenuBar(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle);
	~CGUIDecentralisedMenuBar();

	//! draws the element and its children
	virtual void draw();

	//! called if an event happened.
	virtual bool OnEvent(const SEvent& event);

	//! Updates the absolute position.
	virtual void updateAbsolutePosition();

	virtual u32 addRightItem(const wchar_t* text, s32 commandid,
		bool enabled, bool hasSubMenu, bool checked, bool autoChecking);

	virtual u32 insertRightItem(u32 idx, const wchar_t* text, s32 commandId, bool enabled,
		bool hasSubMenu, bool checked, bool autoChecking);

protected:

	core::array<SItem> ItemsRight;
	s32 HighLightedRight;

	virtual void recalculateSize();

	//! returns the item highlight-area
	virtual core::rect<s32> getHRect(const SItem& i, const core::rect<s32>& absolute) const;

	//! Gets drawing rect of Item
	virtual core::rect<s32> getRect(const SItem& i, const core::rect<s32>& absolute) const;
};

#endif