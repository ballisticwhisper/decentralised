#include "CGUIConsoleRow.h"
#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUISkin.h"
#include "IGUIEnvironment.h"
#include "IVideoDriver.h"
#include "IGUIFont.h"
#include "os.h"

namespace irr
{
	namespace gui
	{

		//! constructor
		CGUIConsoleRow::CGUIConsoleRow(core::stringw text,
			video::SColor color,
			IGUIFont* font,
			IGUIEnvironment* environment,
			IGUIElement* parent,
			s32 id,
			core::rect<s32> rectangle,
			s32 timeFadeIn,
			s32 timeVisible,
			s32 timeFadeOut)
			: IGUIConsoleRow(text, color, font, environment, parent, id, rectangle, timeFadeIn, timeVisible, timeFadeOut), OverrideFont(0)
		{
#ifdef _DEBUG
			setDebugName("CGUIConsoleRow");
#endif
			setTabStop(false);
			setTabOrder(-1);
			timeFadeIn_ = timeFadeIn;
			timeVisible_ = timeVisible;
			timeFadeOut_ = timeFadeOut;
			color_ = color;
			color_.setAlpha(0);
			IsEnabled = true;
			action_ = EFA_FADE_IN;
			startTime_ = os::Timer::getTime();
			endTime_ = startTime_ + timeFadeIn_;
			Text = text;
			font_ = font;
			setAlignment(EGUI_ALIGNMENT::EGUIA_UPPERLEFT, EGUI_ALIGNMENT::EGUIA_UPPERLEFT,
				EGUI_ALIGNMENT::EGUIA_LOWERRIGHT, EGUI_ALIGNMENT::EGUIA_LOWERRIGHT);
		}


		//! destructor
		CGUIConsoleRow::~CGUIConsoleRow()
		{
			if (OverrideFont)
				OverrideFont->drop();
		}

		//! called if an event happened.
		bool CGUIConsoleRow::OnEvent(const SEvent& event)
		{
			if (!isEnabled())
				return IGUIElement::OnEvent(event);

			switch (event.EventType)
			{
			case EET_KEY_INPUT_EVENT:
				break;
			case EET_GUI_EVENT:
				break;
			case EET_MOUSE_INPUT_EVENT:
				break;
			default:
				break;
			}

			return Parent ? Parent->OnEvent(event) : false;
		}

		bool CGUIConsoleRow::IsDisposed()
		{
			if (action_ == EFA_DISPOSING)
				return true;

			return false;
		}

		//! draws the element and its children
		void CGUIConsoleRow::draw()
		{
			if (action_ == EFA_DISPOSING)
				return;

			if (action_ == EFA_FADE_IN)
			{
				u32 now = os::Timer::getTime();
				u32 alpha = ((float)(now - startTime_) / timeFadeIn_) * 255;
				color_.setAlpha(alpha);

				if (now > endTime_)
				{
					action_ = EFA_PAUSE;
					startTime_ = os::Timer::getTime();
					endTime_ = startTime_ + timeVisible_;
				}
			}
			else if (action_ == EFA_PAUSE)
			{
				u32 now = os::Timer::getTime();
				if (now > endTime_)
				{
					action_ = EFA_FADE_OUT;
					startTime_ = os::Timer::getTime();
					endTime_ = startTime_ + timeFadeOut_;
				}
			}
			else if (action_ == EFA_FADE_OUT)
			{
				u32 now = os::Timer::getTime();
				u32 alpha = 255 - ((float)(now - startTime_) / timeFadeOut_) * 255;
				color_.setAlpha(alpha);

				if (now > endTime_)
				{					
					action_ = EFA_DISPOSING;
					startTime_ = 0;
					endTime_ = 0;

					color_.setAlpha(0);

					IsVisible = false;
					IsEnabled = false;

					SEvent disposeEvent;
					disposeEvent.EventType = EEVENT_TYPE::EET_USER_EVENT;
					disposeEvent.GUIEvent.Element = this;
					disposeEvent.GUIEvent.EventType = EGET_ELEMENT_CLOSED;
					OnEvent(disposeEvent);

					return;
				}
			}
			else
				color_.setAlpha(255);

			video::IVideoDriver* driver = Environment->getVideoDriver();

			core::rect<s32> frameRect(AbsoluteRect);
			frameRect.UpperLeftCorner.Y -= 5;
			frameRect.LowerRightCorner.Y -= 5;

			//driver->draw2DRectangle(SColor(255, 255, 0, 0), frameRect);
			font_->draw(Text.c_str(), frameRect, color_, false, true, &frameRect);

			IGUIElement::draw();
		}


		//! sets another skin independent font. if this is set to zero, the button uses the font of the skin.
		void CGUIConsoleRow::setOverrideFont(IGUIFont* font)
		{
			if (OverrideFont == font)
				return;

			if (OverrideFont)
				OverrideFont->drop();

			OverrideFont = font;

			if (OverrideFont)
				OverrideFont->grab();
		}
	} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

