#ifndef __I_GUI_CONSOLE_OVERLAY_H_INCLUDED__
#define __I_GUI_CONSOLE_OVERLAY_H_INCLUDED__

#include "IGUIElement.h"
#include "IGUIFont.h"

namespace irr
{
	namespace gui
	{
		class IGUIConsoleOverlay : public IGUIElement
		{
		public:

			//! constructor
			IGUIConsoleOverlay(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle,
				irr::video::ITexture* topLeft = NULL, irr::video::ITexture* topRight = NULL,
				irr::video::ITexture* bottomLeft = NULL, irr::video::ITexture* bottomRight = NULL)
				: IGUIElement(EGUI_ELEMENT_TYPE::EGUIET_STATIC_TEXT, environment, parent, id, rectangle) {}

			virtual void setOverrideFont(IGUIFont* font = 0) = 0;

			virtual IGUIFont* getOverrideFont(void) const = 0;

			virtual IGUIFont* getActiveFont() const = 0;

			virtual void addLine(video::SColor color, core::stringw text) = 0;
		};
	} // end namespace gui
} // end namespace irr

#endif

