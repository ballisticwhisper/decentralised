#ifndef __C_GUI_CONSOLE_ROW_H_INCLUDED__
#define __C_GUI_CONSOLE_ROW_H_INCLUDED__

#include <mutex>
#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUIConsoleRow.h"
#include "CGUIStaticText.h"
#include "CGUIInOutFader.h"
#include "CGUIDecentralisedSkin.h"

namespace irr
{
	namespace gui
	{
		class CGUIConsoleRow : public IGUIConsoleRow
		{
		public:

			//! constructor
			CGUIConsoleRow(core::stringw text,
				video::SColor color,
				IGUIFont* font,
				IGUIEnvironment* environment,
				IGUIElement* parent,
				s32 id,
				core::rect<s32> rectangle,
				s32 timeFadeIn,
				s32 timeVisible,
				s32 timeFadeOut);

			//! destructor
			virtual ~CGUIConsoleRow();

			//! called if an event happened.
			virtual bool OnEvent(const SEvent& event);

			//! draws the element and its children
			virtual void draw();

			//! sets another skin independent font. if this is set to zero, the button uses the font of the skin.
			virtual void setOverrideFont(IGUIFont* font = 0);

			bool IsDisposed();

		private:

			enum RowFadeAction
			{
				EFA_NOTHING = 0,
				EFA_PAUSE,
				EFA_FADE_IN,
				EFA_FADE_OUT,
				EFA_DISPOSING
			};

			u32 startTime_;
			u32 endTime_;
			RowFadeAction action_;

			s32 timeFadeIn_;
			s32 timeVisible_;
			s32 timeFadeOut_;

				IGUIFont* OverrideFont;
			video::SColor color_;
			std::mutex m;
			IGUIFont* font_;
		};

	} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

#endif 

