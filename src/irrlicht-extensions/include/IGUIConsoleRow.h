#ifndef __I_GUI_CONSOLE_ROW_H_INCLUDED__
#define __I_GUI_CONSOLE_ROW_H_INCLUDED__

#include "IGUIElement.h"
#include "IGUIFont.h"

namespace irr
{
	namespace gui
	{
		class IGUIConsoleRow : public IGUIElement
		{
		public:

			//! constructor
			IGUIConsoleRow(core::stringw text, 
						   video::SColor color, 
						   IGUIFont* font, 
						   IGUIEnvironment* environment, 
						   IGUIElement* parent, 
						   s32 id, 
						   core::rect<s32> rectangle,
   						   s32 timeFadeIn,
						   s32 timeVisible,
						   s32 timeFadeOut)
				: IGUIElement(EGUI_ELEMENT_TYPE::EGUIET_STATIC_TEXT, environment, parent, id, rectangle) {}

			virtual bool IsDisposed() { return false;  }

		};
	} // end namespace gui
} // end namespace irr

#endif

