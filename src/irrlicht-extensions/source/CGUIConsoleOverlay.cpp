#include "CGUIConsoleOverlay.h"
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
		CGUIConsoleOverlay::CGUIConsoleOverlay(IGUIEnvironment* environment, IGUIElement* parent,
			s32 id, core::rect<s32> rectangle,
			irr::video::ITexture* topLeft, irr::video::ITexture* topRight,
			irr::video::ITexture* bottomLeft, irr::video::ITexture* bottomRight)
			: IGUIConsoleOverlay(environment, parent, id, rectangle), OverrideFont(0)
		{
#ifdef _DEBUG
			setDebugName("CGUIConsoleOverlay");
#endif
			ConsoleTopLeft = topLeft;
			ConsoleTopRight = topRight;
			ConsoleBottomLeft = bottomLeft;
			ConsoleBottomRight = bottomRight;

			// This element can be tabbed.
			setTabStop(false);
			setTabOrder(-1);
			IsEnabled = true;
			setAlignment(EGUI_ALIGNMENT::EGUIA_UPPERLEFT, EGUI_ALIGNMENT::EGUIA_UPPERLEFT, 
					     EGUI_ALIGNMENT::EGUIA_LOWERRIGHT, EGUI_ALIGNMENT::EGUIA_LOWERRIGHT);
		}


		//! destructor
		CGUIConsoleOverlay::~CGUIConsoleOverlay()
		{
			if (OverrideFont)
				OverrideFont->drop();
		}

		//! called if an event happened.
		bool CGUIConsoleOverlay::OnEvent(const SEvent& event)
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
			case EET_USER_EVENT:
				if (event.GUIEvent.EventType == EGET_ELEMENT_CLOSED)
				{
					disposedLock.lock();

					IGUIElement* elem = event.GUIEvent.Element;
					disposedChildren.push_back(elem);

					disposedLock.unlock();
				}
				break;
			default:
				break;
			}

			return Parent ? Parent->OnEvent(event) : false;
		}


		//! draws the element and its children
		void CGUIConsoleOverlay::draw()
		{
			if (!IsVisible)
				return;

			disposedLock.lock();

			for (list<IGUIElement*>::ConstIterator i = disposedChildren.begin(); i != disposedChildren.end(); ++i)
				removeChild(*i);

			disposedChildren.clear();

			disposedLock.unlock();

			if (Children.size() == 0)
				return;

			IGUISkin* skin = Environment->getSkin();
			video::IVideoDriver* driver = Environment->getVideoDriver();

			core::rect<s32> backingRect = getBackingRect();

			video::SColor col[4];
			col[0] = video::SColor(128, 0, 0, 0);
			col[1] = video::SColor(128, 0, 0, 0);
			col[2] = video::SColor(128, 0, 0, 0);
			col[3] = video::SColor(128, 0, 0, 0);

			core::dimension2d<u32> leftTopSize(0, 0);
			if (ConsoleTopLeft)
			{
				leftTopSize = ConsoleTopLeft->getSize();
				core::rect<s32> leftRect = backingRect;
				leftRect.LowerRightCorner.X = leftRect.UpperLeftCorner.X + leftTopSize.Width;
				leftRect.LowerRightCorner.Y = leftRect.UpperLeftCorner.Y + leftTopSize.Height;

				driver->draw2DImage(ConsoleTopLeft,
					leftRect,
					core::rect<s32>(0, 0, leftTopSize.Width, leftTopSize.Height), &leftRect,
					col, true);
			}

			core::dimension2d<u32> leftBottomSize(0, 0);
			if (ConsoleBottomLeft)
			{
				leftBottomSize = ConsoleBottomLeft->getSize();
				core::rect<s32> leftRect = backingRect;
				leftRect.LowerRightCorner.X = leftRect.UpperLeftCorner.X + leftBottomSize.Width;
				leftRect.UpperLeftCorner.Y = leftRect.LowerRightCorner.Y - leftBottomSize.Height;

				driver->draw2DImage(ConsoleBottomLeft,
					leftRect,
					core::rect<s32>(0, 0, leftBottomSize.Width, leftBottomSize.Height), &leftRect,
					col, true);
			}

			core::dimension2d<u32> rightTopSize(0, 0);
			if (ConsoleTopRight)
			{
				rightTopSize = ConsoleTopRight->getSize();
				core::rect<s32> leftRect = backingRect;
				leftRect.UpperLeftCorner.X = leftRect.LowerRightCorner.X - rightTopSize.Width;
				leftRect.LowerRightCorner.Y = leftRect.UpperLeftCorner.Y + rightTopSize.Height;

				driver->draw2DImage(ConsoleTopRight,
					leftRect,
					core::rect<s32>(0, 0, rightTopSize.Width, rightTopSize.Height), &leftRect,
					col, true);
			}

			core::dimension2d<u32> rightBottomSize(0, 0);
			if (ConsoleBottomRight)
			{
				rightBottomSize = ConsoleBottomRight->getSize();
				core::rect<s32> leftRect = backingRect;
				leftRect.UpperLeftCorner.X = leftRect.LowerRightCorner.X - rightBottomSize.Width;
				leftRect.UpperLeftCorner.Y = leftRect.LowerRightCorner.Y - rightBottomSize.Height;

				driver->draw2DImage(ConsoleBottomRight,
					leftRect,
					core::rect<s32>(0, 0, rightBottomSize.Width, rightBottomSize.Height), &leftRect,
					col, true);
			}


			core::rect<s32> leftSideRect = backingRect;
			leftSideRect.UpperLeftCorner.Y += leftTopSize.Height;
			leftSideRect.LowerRightCorner.Y -= leftBottomSize.Height;
			leftSideRect.LowerRightCorner.X = leftSideRect.UpperLeftCorner.X + leftTopSize.Width;

			driver->draw2DRectangle(col[0], leftSideRect);

			leftSideRect = backingRect;
			leftSideRect.UpperLeftCorner.X = leftTopSize.Width + 10;
			leftSideRect.LowerRightCorner.X -= rightTopSize.Width;

			driver->draw2DRectangle(col[0], leftSideRect);

			leftSideRect = backingRect;
			leftSideRect.UpperLeftCorner.Y += rightTopSize.Height;
			leftSideRect.LowerRightCorner.Y -= rightBottomSize.Height;
			leftSideRect.UpperLeftCorner.X = leftSideRect.LowerRightCorner.X - rightTopSize.Width;

			driver->draw2DRectangle(col[0], leftSideRect);

			IGUIElement::draw();
		}


		//! sets another skin independent font. if this is set to zero, the button uses the font of the skin.
		void CGUIConsoleOverlay::setOverrideFont(IGUIFont* font)
		{
			if (OverrideFont == font)
				return;

			if (OverrideFont)
				OverrideFont->drop();

			OverrideFont = font;

			if (OverrideFont)
				OverrideFont->grab();
		}

		//! Gets the override font (if any)
		IGUIFont * CGUIConsoleOverlay::getOverrideFont() const
		{
			return OverrideFont;
		}

		//! Get the font which is used right now for drawing
		IGUIFont* CGUIConsoleOverlay::getActiveFont() const
		{
			if (OverrideFont)
				return OverrideFont;
			IGUISkin* skin = Environment->getSkin();
			if (skin)
				return skin->getFont(EGDF_BUTTON);
			return 0;
		}

		void CGUIConsoleOverlay::addLine(video::SColor color, core::stringw text)
		{
			m.lock();

			IGUIFont* font = getActiveFont();

			core::rect<s32> bottomLineRect(AbsoluteRect);
			s32 width = AbsoluteRect.LowerRightCorner.X - AbsoluteRect.UpperLeftCorner.X;

			core::dimension2d<u32> textSize = font->getDimension(text.c_str());
			bottomLineRect.UpperLeftCorner.X = 0;
			bottomLineRect.UpperLeftCorner.Y = bottomLineRect.LowerRightCorner.Y - textSize.Height - 7; // 5 padding
			if (textSize.Width < (u32)width)
				bottomLineRect.LowerRightCorner.X = textSize.Width + 20;
			else
				bottomLineRect.LowerRightCorner.X = width - 10;

			core::list<IGUIElement*>::Iterator it = Children.begin();
			for (; it != Children.end(); ++it)
			{
				core::rect<s32> absoluteRect = (*it)->getAbsoluteClippingRect();
				(*it)->setRelativePosition(core::position2di(0, absoluteRect.UpperLeftCorner.Y - textSize.Height - 7));
			}

			IGUIConsoleRow* elem = new CGUIConsoleRow(text.c_str(), color, font, this->Environment, this, -1, bottomLineRect, 900, 10000, 1500);
			addChild(elem);
			elem->drop();

			m.unlock();
		}

		//! Writes attributes of the element.
		void CGUIConsoleOverlay::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options = 0) const
		{
			IGUIConsoleOverlay::serializeAttributes(out, options);

			//out->addTexture("Image", Image);
			//out->addRect("ImageRect", ImageRect);
			//out->addTexture("PressedImage", PressedImage);
			//out->addRect("PressedImageRect", PressedImageRect);

			//out->addBool("UseAlphaChannel", isAlphaChannelUsed());
			//out->addBool("Border", isDrawingBorder());
			//out->addBool("ScaleImage", isScalingImage());

			//   out->addString  ("OverrideFont",	OverrideFont);
		}


		//! Reads attributes of the element
		void CGUIConsoleOverlay::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options = 0)
		{
			IGUIConsoleOverlay::deserializeAttributes(in, options);

			//IsPushButton = in->getAttributeAsBool("PushButton");
			//Pressed = IsPushButton ? in->getAttributeAsBool("Pressed") : false;

			//core::rect<s32> rec = in->getAttributeAsRect("ImageRect");
			//if (rec.isValid())
			//	setImage(in->getAttributeAsTexture("Image"), rec);
			//else
			//	setImage(in->getAttributeAsTexture("Image"));

			//rec = in->getAttributeAsRect("PressedImageRect");
			//if (rec.isValid())
			//	setPressedImage(in->getAttributeAsTexture("PressedImage"), rec);
			//else
			//	setPressedImage(in->getAttributeAsTexture("PressedImage"));

			//setDrawBorder(in->getAttributeAsBool("Border"));
			//setUseAlphaChannel(in->getAttributeAsBool("UseAlphaChannel"));
			//setScaleImage(in->getAttributeAsBool("ScaleImage"));

			////   setOverrideFont(in->getAttributeAsString("OverrideFont"));

			updateAbsolutePosition();
		}

		core::rect<s32> CGUIConsoleOverlay::getBackingRect()
		{
			IGUIFont* font = getActiveFont();

			s32 maxWidth = 0;
			s32 totalHeight = 0;

			core::list<IGUIElement*>::Iterator it = Children.begin();
			for (; it != Children.end(); ++it)
			{
				core::rect<s32> absoluteRect = (*it)->getAbsoluteClippingRect();

				s32 width = absoluteRect.LowerRightCorner.X - absoluteRect.UpperLeftCorner.X;
				s32 height = absoluteRect.LowerRightCorner.Y - absoluteRect.UpperLeftCorner.Y;
				if (width > maxWidth)
					maxWidth = width;

				totalHeight += height;
			}

			core::rect<s32> backingRect = AbsoluteRect;
			backingRect.UpperLeftCorner.X = 10;
			backingRect.UpperLeftCorner.Y = backingRect.LowerRightCorner.Y - (totalHeight + 10);
			backingRect.LowerRightCorner.X = maxWidth + 10;

			return backingRect;
		}

	} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

