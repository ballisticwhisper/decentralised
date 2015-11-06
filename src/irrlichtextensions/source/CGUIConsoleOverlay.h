#ifndef __C_GUI_VOO_CONSOLE_OVERLAY_H_INCLUDED__
#define __C_GUI_VOO_CONSOLE_OVERLAY_H_INCLUDED__

#include <mutex>
#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUIConsoleOverlay.h"
#include "CGUIStaticText.h"

namespace irr
{
	namespace gui
	{
		class CGUIConsoleOverlay : public IGUIConsoleOverlay
		{
		public:

			//! constructor
			CGUIConsoleOverlay(IGUIEnvironment* environment, IGUIElement* parent,
				s32 id, core::rect<s32> rectangle,
				irr::video::ITexture* topLeft = NULL, irr::video::ITexture* topRight = NULL,
				irr::video::ITexture* bottomLeft = NULL, irr::video::ITexture* bottomRight = NULL);

			//! destructor
			virtual ~CGUIConsoleOverlay();

			//! called if an event happened.
			virtual bool OnEvent(const SEvent& event);

			//! draws the element and its children
			virtual void draw();

			//! sets another skin independent font. if this is set to zero, the button uses the font of the skin.
			virtual void setOverrideFont(IGUIFont* font = 0);

			//! Gets the override font (if any)
			virtual IGUIFont* getOverrideFont() const;

			//! Get the font which is used right now for drawing
			virtual IGUIFont* getActiveFont() const;

			//! Writes attributes of the element.
			virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const;

			//! Reads attributes of the element
			virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

			virtual void addLine(video::SColor color, core::stringw text);

		private:

			IGUIFont* OverrideFont;
			irr::video::ITexture* ConsoleTopLeft;
			irr::video::ITexture* ConsoleTopRight;
			irr::video::ITexture* ConsoleBottomLeft;
			irr::video::ITexture* ConsoleBottomRight;
			std::mutex m;

			virtual core::rect<s32> getBackingRect();
		};

	} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

#endif 

