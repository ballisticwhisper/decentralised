#include "CGUIDecentralisedButton.h"
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
		CGUIDecentralisedButton::CGUIDecentralisedButton(IGUIEnvironment* environment, IGUIElement* parent,
			s32 id, core::rect<s32> rectangle)
			: IGUIButton(environment, parent, id, rectangle),
			SpriteBank(0), OverrideFont(0), Image(0), PressedImage(0),
			ClickTime(0), HoverTime(0), FocusTime(0),
			IsPushButton(false), Pressed(false),
			UseAlphaChannel(false), DrawBorder(false), ScaleImage(false), IsAutoSize(true), AutoSizeExpandLeft(false)
		{
#ifdef _DEBUG
			setDebugName("CGUIDecentralisedButton");
#endif
			// Initialize the sprites.
			for (u32 i = 0; i < EGBS_COUNT; ++i)
				ButtonSprites[i].Index = -1;

			// This element can be tabbed.
			setTabStop(true);
			setTabOrder(-1);

			BackLeft = NULL;
			BackMiddle = NULL;
			BackRight = NULL;

			PressedLeft = NULL;
			PressedMiddle = NULL;
			PressedRight = NULL;
		}


		//! destructor
		CGUIDecentralisedButton::~CGUIDecentralisedButton()
		{
			if (OverrideFont)
				OverrideFont->drop();

			if (Image)
				Image->drop();

			if (PressedImage)
				PressedImage->drop();

			if (SpriteBank)
				SpriteBank->drop();
		}


		//! Sets if the images should be scaled to fit the button
		void CGUIDecentralisedButton::setScaleImage(bool scaleImage)
		{
			ScaleImage = scaleImage;
		}


		//! Returns whether the button scale the used images
		bool CGUIDecentralisedButton::isScalingImage() const
		{
			_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
			return ScaleImage;
		}


		//! Sets if the button should use the skin to draw its border
		void CGUIDecentralisedButton::setDrawBorder(bool border)
		{
			DrawBorder = border;
		}


		void CGUIDecentralisedButton::setSpriteBank(IGUISpriteBank* sprites)
		{
			if (sprites)
				sprites->grab();

			if (SpriteBank)
				SpriteBank->drop();

			SpriteBank = sprites;
		}


		void CGUIDecentralisedButton::setSprite(EGUI_BUTTON_STATE state, s32 index, video::SColor color, bool loop)
		{
			if (SpriteBank)
			{
				ButtonSprites[(u32)state].Index = index;
				ButtonSprites[(u32)state].Color = color;
				ButtonSprites[(u32)state].Loop = loop;
			}
			else
			{
				ButtonSprites[(u32)state].Index = -1;
			}
		}


		//! called if an event happened.
		bool CGUIDecentralisedButton::OnEvent(const SEvent& event)
		{
			if (!isEnabled())
				return IGUIElement::OnEvent(event);

			switch (event.EventType)
			{
			case EET_KEY_INPUT_EVENT:
				if (event.KeyInput.PressedDown &&
					(event.KeyInput.Key == KEY_RETURN || event.KeyInput.Key == KEY_SPACE))
				{
					if (!IsPushButton)
						setPressed(true);
					else
						setPressed(!Pressed);

					return true;
				}
				if (Pressed && !IsPushButton && event.KeyInput.PressedDown && event.KeyInput.Key == KEY_ESCAPE)
				{
					setPressed(false);
					return true;
				}
				else
					if (!event.KeyInput.PressedDown && Pressed &&
						(event.KeyInput.Key == KEY_RETURN || event.KeyInput.Key == KEY_SPACE))
					{

						if (!IsPushButton)
							setPressed(false);

						if (Parent)
						{
							SEvent newEvent;
							newEvent.EventType = EET_GUI_EVENT;
							newEvent.GUIEvent.Caller = this;
							newEvent.GUIEvent.Element = 0;
							newEvent.GUIEvent.EventType = EGET_BUTTON_CLICKED;
							Parent->OnEvent(newEvent);
						}
						return true;
					}
				break;
			case EET_GUI_EVENT:
				if (event.GUIEvent.Caller == this)
				{
					if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
					{
						if (!IsPushButton)
							setPressed(false);
						FocusTime = os::Timer::getTime();
					}
					else if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUSED)
					{
						FocusTime = os::Timer::getTime();
					}
					else if (event.GUIEvent.EventType == EGET_ELEMENT_HOVERED || event.GUIEvent.EventType == EGET_ELEMENT_LEFT)
					{
						HoverTime = os::Timer::getTime();
					}
				}
				break;
			case EET_MOUSE_INPUT_EVENT:
				if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
				{
					if (Environment->hasFocus(this) &&
						!AbsoluteClippingRect.isPointInside(core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y)))
					{
						Environment->removeFocus(this);
						return false;
					}

					if (!IsPushButton)
						setPressed(true);

					Environment->setFocus(this);
					return true;
				}
				else
					if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
					{
						bool wasPressed = Pressed;

						if (!AbsoluteClippingRect.isPointInside(core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y)))
						{
							if (!IsPushButton)
								setPressed(false);
							return true;
						}

						if (!IsPushButton)
							setPressed(false);
						else
						{
							setPressed(!Pressed);
						}

						if ((!IsPushButton && wasPressed && Parent) ||
							(IsPushButton && wasPressed != Pressed))
						{
							SEvent newEvent;
							newEvent.EventType = EET_GUI_EVENT;
							newEvent.GUIEvent.Caller = this;
							newEvent.GUIEvent.Element = 0;
							newEvent.GUIEvent.EventType = EGET_BUTTON_CLICKED;
							Parent->OnEvent(newEvent);
						}

						return true;
					}
				break;
			default:
				break;
			}

			return Parent ? Parent->OnEvent(event) : false;
		}


		//! draws the element and its children
		void CGUIDecentralisedButton::draw()
		{
			if (!IsVisible)
				return;

			IGUISkin* skin = Environment->getSkin();
			video::IVideoDriver* driver = Environment->getVideoDriver();

			if (Text.size() && IsAutoSize)
			{
				IGUIFont* font = getActiveFont();
				core::dimension2d<u32> textSize = font->getDimension(Text.c_str());

				if (AutoSizeExpandLeft)
					AbsoluteRect.UpperLeftCorner.X = AbsoluteRect.LowerRightCorner.X - textSize.Width - 20;
				else
					AbsoluteRect.LowerRightCorner.X = AbsoluteRect.UpperLeftCorner.X + textSize.Width + 20;

				AbsoluteClippingRect = AbsoluteRect;
			}

			// todo:	move sprite up and text down if the pressed state has a sprite
			const core::position2di spritePos = AbsoluteRect.getCenter();

			if (!Pressed)
			{
				core::dimension2d<u32> leftSize(0, 0);
				if (BackLeft)
				{

					leftSize = BackLeft->getSize();
					core::rect<s32> leftRect = AbsoluteRect;
					leftRect.LowerRightCorner.X = leftRect.UpperLeftCorner.X + leftSize.Width;

					driver->draw2DImage(BackLeft,
						leftRect,
						core::rect<s32>(0, 0, leftSize.Width, leftSize.Height), &AbsoluteClippingRect,
						0, true);
				}

				core::dimension2d<u32> rightSize(0, 0);
				if (BackRight)
				{
					rightSize = BackRight->getSize();
					core::rect<s32> rightRect = AbsoluteRect;
					rightRect.UpperLeftCorner.X = rightRect.LowerRightCorner.X - rightSize.Width;

					driver->draw2DImage(BackRight,
						rightRect,
						core::rect<s32>(0, 0, rightSize.Width, rightSize.Height), &AbsoluteClippingRect,
						0, true);
				}

				if (BackMiddle)
				{
					core::dimension2d<u32> middleSize = BackMiddle->getSize();
					core::rect<s32> middleRect = AbsoluteRect;
					middleRect.UpperLeftCorner.X += leftSize.Width;
					middleRect.LowerRightCorner.X -= rightSize.Width;

					driver->draw2DImage(BackMiddle,
						middleRect,
						core::rect<s32>(0, 0, middleSize.Width, middleSize.Height), &AbsoluteClippingRect,
						0, true);
				}


				if (DrawBorder)
					skin->draw3DButtonPaneStandard(this, AbsoluteRect, &AbsoluteClippingRect);

				if (Image)
				{
					core::position2d<s32> pos = spritePos;
					pos.X -= ImageRect.getWidth() / 2;
					pos.Y -= ImageRect.getHeight() / 2;

					driver->draw2DImage(Image,
						ScaleImage ? AbsoluteRect :
						core::recti(pos, ImageRect.getSize()),
						ImageRect, &AbsoluteClippingRect,
						0, UseAlphaChannel);
				}
			}
			else
			{
				core::dimension2d<u32> leftSize(0, 0);
				if (PressedLeft)
				{
					leftSize = PressedLeft->getSize();
					core::rect<s32> leftRect = AbsoluteRect;
					leftRect.LowerRightCorner.X = leftRect.UpperLeftCorner.X + leftSize.Width;

					driver->draw2DImage(PressedLeft,
						leftRect,
						core::rect<s32>(0, 0, leftSize.Width, leftSize.Height), &AbsoluteClippingRect,
						0, true);
				}

				core::dimension2d<u32> rightSize(0, 0);
				if (PressedRight)
				{
					rightSize = PressedRight->getSize();
					core::rect<s32> rightRect = AbsoluteRect;
					rightRect.UpperLeftCorner.X = rightRect.LowerRightCorner.X - rightSize.Width;

					driver->draw2DImage(PressedRight,
						rightRect,
						core::rect<s32>(0, 0, rightSize.Width, rightSize.Height), &AbsoluteClippingRect,
						0, true);
				}

				if (PressedMiddle)
				{
					core::dimension2d<u32> middleSize = PressedMiddle->getSize();
					core::rect<s32> middleRect = AbsoluteRect;
					middleRect.UpperLeftCorner.X += leftSize.Width;
					middleRect.LowerRightCorner.X -= rightSize.Width;

					driver->draw2DImage(PressedMiddle,
						middleRect,
						core::rect<s32>(0, 0, middleSize.Width, middleSize.Height), &AbsoluteClippingRect,
						0, true);
				}


				if (DrawBorder)
					skin->draw3DButtonPanePressed(this, AbsoluteRect, &AbsoluteClippingRect);

				if (PressedImage)
				{
					core::position2d<s32> pos = spritePos;
					pos.X -= PressedImageRect.getWidth() / 2;
					pos.Y -= PressedImageRect.getHeight() / 2;

					if (Image == PressedImage && PressedImageRect == ImageRect)
					{
						pos.X += skin->getSize(EGDS_BUTTON_PRESSED_IMAGE_OFFSET_X);
						pos.Y += skin->getSize(EGDS_BUTTON_PRESSED_IMAGE_OFFSET_Y);
					}
					driver->draw2DImage(PressedImage,
						ScaleImage ? AbsoluteRect :
						core::recti(pos, PressedImageRect.getSize()),
						PressedImageRect, &AbsoluteClippingRect,
						0, UseAlphaChannel);
				}
			}

			if (SpriteBank)
			{
				// pressed / unpressed animation
				u32 state = Pressed ? (u32)EGBS_BUTTON_DOWN : (u32)EGBS_BUTTON_UP;
				if (ButtonSprites[state].Index != -1)
				{
					SpriteBank->draw2DSprite(ButtonSprites[state].Index, spritePos,
						&AbsoluteClippingRect, ButtonSprites[state].Color, ClickTime, os::Timer::getTime(),
						ButtonSprites[state].Loop, true);
				}

				// focused / unfocused animation
				state = Environment->hasFocus(this) ? (u32)EGBS_BUTTON_FOCUSED : (u32)EGBS_BUTTON_NOT_FOCUSED;
				if (ButtonSprites[state].Index != -1)
				{
					SpriteBank->draw2DSprite(ButtonSprites[state].Index, spritePos,
						&AbsoluteClippingRect, ButtonSprites[state].Color, FocusTime, os::Timer::getTime(),
						ButtonSprites[state].Loop, true);
				}

				// mouse over / off animation
				if (isEnabled())
				{
					state = Environment->getHovered() == this ? (u32)EGBS_BUTTON_MOUSE_OVER : (u32)EGBS_BUTTON_MOUSE_OFF;
					if (ButtonSprites[state].Index != -1)
					{
						SpriteBank->draw2DSprite(ButtonSprites[state].Index, spritePos,
							&AbsoluteClippingRect, ButtonSprites[state].Color, HoverTime, os::Timer::getTime(),
							ButtonSprites[state].Loop, true);
					}
				}
			}

			if (Text.size())
			{
				IGUIFont* font = getActiveFont();

				core::rect<s32> rect = AbsoluteRect;
				if (Pressed)
				{
					rect.UpperLeftCorner.X += skin->getSize(EGDS_BUTTON_PRESSED_TEXT_OFFSET_X);
					rect.UpperLeftCorner.Y += skin->getSize(EGDS_BUTTON_PRESSED_TEXT_OFFSET_Y);
				}

				if (font)
					font->draw(Text.c_str(), rect,
					skin->getColor(isEnabled() ? EGDC_BUTTON_TEXT : EGDC_GRAY_TEXT),
					true, true, &AbsoluteClippingRect);
			}

			IGUIElement::draw();
		}


		//! sets another skin independent font. if this is set to zero, the button uses the font of the skin.
		void CGUIDecentralisedButton::setOverrideFont(IGUIFont* font)
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
		IGUIFont * CGUIDecentralisedButton::getOverrideFont() const
		{
			return OverrideFont;
		}

		//! Get the font which is used right now for drawing
		IGUIFont* CGUIDecentralisedButton::getActiveFont() const
		{
			if (OverrideFont)
				return OverrideFont;
			IGUISkin* skin = Environment->getSkin();
			if (skin)
				return skin->getFont(EGDF_BUTTON);
			return 0;
		}

		//! Sets an image which should be displayed on the button when it is in normal state.
		void CGUIDecentralisedButton::setImage(video::ITexture* image)
		{
			if (image)
				image->grab();
			if (Image)
				Image->drop();

			Image = image;
			if (image)
				ImageRect = core::rect<s32>(core::position2d<s32>(0, 0), image->getOriginalSize());

			if (!PressedImage)
				setPressedImage(Image);
		}


		//! Sets the image which should be displayed on the button when it is in its normal state.
		void CGUIDecentralisedButton::setImage(video::ITexture* image, const core::rect<s32>& pos)
		{
			setImage(image);
			ImageRect = pos;
		}


		//! Sets an image which should be displayed on the button when it is in pressed state.
		void CGUIDecentralisedButton::setPressedImage(video::ITexture* image)
		{
			if (image)
				image->grab();

			if (PressedImage)
				PressedImage->drop();

			PressedImage = image;
			if (image)
				PressedImageRect = core::rect<s32>(core::position2d<s32>(0, 0), image->getOriginalSize());
		}


		//! Sets the image which should be displayed on the button when it is in its pressed state.
		void CGUIDecentralisedButton::setPressedImage(video::ITexture* image, const core::rect<s32>& pos)
		{
			setPressedImage(image);
			PressedImageRect = pos;
		}


		//! Sets if the button should behave like a push button. Which means it
		//! can be in two states: Normal or Pressed. With a click on the button,
		//! the user can change the state of the button.
		void CGUIDecentralisedButton::setIsPushButton(bool isPushButton)
		{
			IsPushButton = isPushButton;
		}


		void CGUIDecentralisedButton::setAutoSize(bool isAutoSize)
		{
			IsAutoSize = isAutoSize;
		}

		void CGUIDecentralisedButton::setAutoExpand(bool isRight)
		{
			if (isRight)
				AutoSizeExpandLeft = false;
			else
				AutoSizeExpandLeft = true;
		}

		//! Returns if the button is currently pressed
		bool CGUIDecentralisedButton::isPressed() const
		{
			_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
			return Pressed;
		}


		//! Sets the pressed state of the button if this is a pushbutton
		void CGUIDecentralisedButton::setPressed(bool pressed)
		{
			if (Pressed != pressed)
			{
				ClickTime = os::Timer::getTime();
				Pressed = pressed;
			}
		}


		//! Returns whether the button is a push button
		bool CGUIDecentralisedButton::isPushButton() const
		{
			_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
			return IsPushButton;
		}


		//! Sets if the alpha channel should be used for drawing images on the button (default is false)
		void CGUIDecentralisedButton::setUseAlphaChannel(bool useAlphaChannel)
		{
			UseAlphaChannel = useAlphaChannel;
		}


		//! Returns if the alpha channel should be used for drawing images on the button
		bool CGUIDecentralisedButton::isAlphaChannelUsed() const
		{
			_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
			return UseAlphaChannel;
		}


		bool CGUIDecentralisedButton::isDrawingBorder() const
		{
			_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
			return DrawBorder;
		}


		//! Writes attributes of the element.
		void CGUIDecentralisedButton::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options = 0) const
		{
			CGUIDecentralisedButton::serializeAttributes(out, options);

			out->addBool("PushButton", IsPushButton);
			if (IsPushButton)
				out->addBool("Pressed", Pressed);

			out->addTexture("Image", Image);
			out->addRect("ImageRect", ImageRect);
			out->addTexture("PressedImage", PressedImage);
			out->addRect("PressedImageRect", PressedImageRect);

			out->addBool("UseAlphaChannel", isAlphaChannelUsed());
			out->addBool("Border", isDrawingBorder());
			out->addBool("ScaleImage", isScalingImage());

			//   out->addString  ("OverrideFont",	OverrideFont);
		}


		//! Reads attributes of the element
		void CGUIDecentralisedButton::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options = 0)
		{
			CGUIDecentralisedButton::deserializeAttributes(in, options);

			IsPushButton = in->getAttributeAsBool("PushButton");
			Pressed = IsPushButton ? in->getAttributeAsBool("Pressed") : false;

			core::rect<s32> rec = in->getAttributeAsRect("ImageRect");
			if (rec.isValid())
				setImage(in->getAttributeAsTexture("Image"), rec);
			else
				setImage(in->getAttributeAsTexture("Image"));

			rec = in->getAttributeAsRect("PressedImageRect");
			if (rec.isValid())
				setPressedImage(in->getAttributeAsTexture("PressedImage"), rec);
			else
				setPressedImage(in->getAttributeAsTexture("PressedImage"));

			setDrawBorder(in->getAttributeAsBool("Border"));
			setUseAlphaChannel(in->getAttributeAsBool("UseAlphaChannel"));
			setScaleImage(in->getAttributeAsBool("ScaleImage"));

			//   setOverrideFont(in->getAttributeAsString("OverrideFont"));

			updateAbsolutePosition();
		}

		void CGUIDecentralisedButton::setImages(video::ITexture* left, video::ITexture* middle, video::ITexture* right,
												video::ITexture* pleft, video::ITexture* pmiddle, video::ITexture* pright)
		{
			BackLeft = left;
			BackMiddle = middle;
			BackRight = right;

			PressedLeft = pleft;
			PressedMiddle = pmiddle;
			PressedRight = pright;
		}

	} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

