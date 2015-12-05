#include "CGUIDecentralisedWindow.h"
#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUISkin.h"
#include "IGUIEnvironment.h"
#include "IVideoDriver.h"
#include "IGUIButton.h"
#include "IGUIFont.h"
#include "IGUIFontBitmap.h"

namespace irr
{
	namespace gui
	{

		//! constructor
		CGUIDecentralisedWindow::CGUIDecentralisedWindow(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle,
			irr::video::ITexture* titleLeft, irr::video::ITexture* titleMiddle, irr::video::ITexture* titleRight,
			irr::video::ITexture* winLeft, irr::video::ITexture* winRight, 
			irr::video::ITexture* bottomLeft, irr::video::ITexture* bottomMiddle, irr::video::ITexture* bottomRight)
			: IGUIDecentralisedWindow(environment, parent, id, rectangle,
				titleLeft, titleMiddle, titleRight,
				winLeft, winRight,
				bottomLeft, bottomMiddle, bottomRight), Dragging(false), IsDraggable(true), DrawBackground(true), DrawTitlebar(true), IsActive(false)
		{
#ifdef _DEBUG
			setDebugName("CGUIWindow");
#endif

			TitleLeft = titleLeft;
			TitleMiddle = titleMiddle;
			TitleRight = titleRight;

			WinLeft = winLeft;
			WinRight = winRight;

			BottomLeft = bottomLeft;
			BottomMiddle = bottomMiddle;
			BottomRight = bottomRight;

			IGUISkin* skin = 0;
			if (environment)
				skin = environment->getSkin();

			CurrentIconColor = video::SColor(255, 255, 255, 255);

			s32 buttonw = 15;
			if (skin)
			{
				buttonw = skin->getSize(EGDS_WINDOW_BUTTON_WIDTH);
			}
			s32 posx = RelativeRect.getWidth() - buttonw - 15;

			CloseButton = new CGUIDecentralisedButton(Environment, this, -1, core::rect<s32>(posx, 13, posx + buttonw, 13 + buttonw));
			//CloseButton = Environment->addButton(core::rect<s32>(posx, 13, posx + buttonw, 13 + buttonw), this, -1,
			//	L"", skin ? skin->getDefaultText(EGDT_WINDOW_CLOSE) : L"Close");
			CloseButton->setSubElement(true);
			CloseButton->setTabStop(false);
			CloseButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
			posx -= buttonw + 2;

			RestoreButton = Environment->addButton(core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1,
				L"", skin ? skin->getDefaultText(EGDT_WINDOW_RESTORE) : L"Restore");
			RestoreButton->setVisible(false);
			RestoreButton->setSubElement(true);
			RestoreButton->setTabStop(false);
			RestoreButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
			posx -= buttonw + 2;

			MinButton = Environment->addButton(core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1,
				L"", skin ? skin->getDefaultText(EGDT_WINDOW_MINIMIZE) : L"Minimize");
			MinButton->setVisible(false);
			MinButton->setSubElement(true);
			MinButton->setTabStop(false);
			MinButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);

			MinButton->grab();
			RestoreButton->grab();
			CloseButton->grab();

			// this element is a tab group
			setTabGroup(true);
			setTabStop(true);
			setTabOrder(-1);

			refreshSprites();
			updateClientRect();
		}


		//! destructor
		CGUIDecentralisedWindow::~CGUIDecentralisedWindow()
		{
			if (MinButton)
				MinButton->drop();

			if (RestoreButton)
				RestoreButton->drop();

			if (CloseButton)
				CloseButton->drop();
		}

		void CGUIDecentralisedWindow::refreshSprites()
		{
			if (!Environment)
				return;
			IGUISkin* skin = Environment->getSkin();
			if (!skin)
				return;

			IGUISpriteBank* sprites = skin->getSpriteBank();
			if (!sprites)
				return;

			CurrentIconColor = skin->getColor(isEnabled() ? EGDC_WINDOW_SYMBOL : EGDC_GRAY_WINDOW_SYMBOL);

			if (sprites)
			{
				CloseButton->setSpriteBank(sprites);
				CloseButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_CLOSE), video::SColor(255,191,191,191));
				CloseButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_CLOSE), CurrentIconColor);

				RestoreButton->setSpriteBank(sprites);
				RestoreButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_RESTORE), CurrentIconColor);
				RestoreButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_RESTORE), CurrentIconColor);

				MinButton->setSpriteBank(sprites);
				MinButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_MINIMIZE), CurrentIconColor);
				MinButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_MINIMIZE), CurrentIconColor);
			}
		}

		//! called if an event happened.
		bool CGUIDecentralisedWindow::OnEvent(const SEvent& event)
		{
			if (isEnabled())
			{

				switch (event.EventType)
				{
				case EET_GUI_EVENT:
					if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
					{
						Dragging = false;
						IsActive = false;
					}
					else
						if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUSED)
						{
						if (Parent && ((event.GUIEvent.Caller == this) || isMyChild(event.GUIEvent.Caller)))
						{
							Parent->bringToFront(this);
							IsActive = true;
						}
						else
						{
							IsActive = false;
						}
						}
						else
							if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
							{
						if (event.GUIEvent.Caller == CloseButton)
						{
							if (Parent)
							{
								// send close event to parent
								SEvent e;
								e.EventType = EET_GUI_EVENT;
								e.GUIEvent.Caller = this;
								e.GUIEvent.Element = 0;
								e.GUIEvent.EventType = EGET_ELEMENT_CLOSED;

								// if the event was not absorbed
								if (!Parent->OnEvent(e))
									remove();

								return true;

							}
							else
							{
								remove();
								return true;
							}
						}
							}
					break;
				case EET_MOUSE_INPUT_EVENT:
					switch (event.MouseInput.Event)
					{
					case EMIE_LMOUSE_PRESSED_DOWN:
						DragStart.X = event.MouseInput.X;
						DragStart.Y = event.MouseInput.Y;
						Dragging = IsDraggable;
						if (Parent)
							Parent->bringToFront(this);
						return true;
					case EMIE_LMOUSE_LEFT_UP:
						Dragging = false;
						return true;
					case EMIE_MOUSE_MOVED:
						if (!event.MouseInput.isLeftPressed())
							Dragging = false;

						if (Dragging)
						{
							// gui window should not be dragged outside its parent
							if (Parent &&
								(event.MouseInput.X < Parent->getAbsolutePosition().UpperLeftCorner.X + 1 ||
								event.MouseInput.Y < Parent->getAbsolutePosition().UpperLeftCorner.Y + 1 ||
								event.MouseInput.X > Parent->getAbsolutePosition().LowerRightCorner.X - 1 ||
								event.MouseInput.Y > Parent->getAbsolutePosition().LowerRightCorner.Y - 1))
								return true;

							move(core::position2d<s32>(event.MouseInput.X - DragStart.X, event.MouseInput.Y - DragStart.Y));
							DragStart.X = event.MouseInput.X;
							DragStart.Y = event.MouseInput.Y;
							return true;
						}
						break;
					default:
						break;
					}
				default:
					break;
				}
			}

			return IGUIElement::OnEvent(event);
		}


		//! Updates the absolute position.
		void CGUIDecentralisedWindow::updateAbsolutePosition()
		{
			IGUIElement::updateAbsolutePosition();
		}


		//! draws the element and its children
		void CGUIDecentralisedWindow::draw()
		{
			if (IsVisible)
			{
				IGUISkin* skin = Environment->getSkin();


				// update each time because the skin is allowed to change this always.
				updateClientRect();

				if (CurrentIconColor != skin->getColor(isEnabled() ? EGDC_WINDOW_SYMBOL : EGDC_GRAY_WINDOW_SYMBOL))
					refreshSprites();

				core::rect<s32> rect = AbsoluteRect;

				// draw body fast
				if (DrawBackground)
				{
					video::IVideoDriver* driver = Environment->getVideoDriver();

					// titlebar
					core::dimension2d<u32> leftSize(0, 0);
					if (TitleLeft)
					{
						leftSize = TitleLeft->getSize();
						core::rect<s32> leftRect = AbsoluteRect;
						leftRect.LowerRightCorner.X = leftRect.UpperLeftCorner.X + leftSize.Width;
						leftRect.LowerRightCorner.Y = leftRect.UpperLeftCorner.Y + leftSize.Height;

						driver->draw2DImage(TitleLeft,
							leftRect,
							core::rect<s32>(0, 0, leftSize.Width, leftSize.Height), &AbsoluteClippingRect,
							0, true);
					}

					core::dimension2d<u32> rightSize(0, 0);
					if (TitleRight)
					{
						rightSize = TitleRight->getSize();
						core::rect<s32> rightRect = AbsoluteRect;
						rightRect.UpperLeftCorner.X = rightRect.LowerRightCorner.X - rightSize.Width;
						rightRect.LowerRightCorner.Y = rightRect.UpperLeftCorner.Y + rightSize.Height;

						driver->draw2DImage(TitleRight,
							rightRect,
							core::rect<s32>(0, 0, rightSize.Width, rightSize.Height), &AbsoluteClippingRect,
							0, true);
					}

					if (TitleMiddle)
					{
						core::dimension2d<u32> middleSize = TitleMiddle->getSize();
						core::rect<s32> middleRect = AbsoluteRect;
						middleRect.UpperLeftCorner.X += leftSize.Width;
						middleRect.LowerRightCorner.X -= rightSize.Width;
						middleRect.LowerRightCorner.Y = middleRect.UpperLeftCorner.Y + middleSize.Height;

						driver->draw2DImage(TitleMiddle,
							middleRect,
							core::rect<s32>(0, 0, middleSize.Width, middleSize.Height), &AbsoluteClippingRect,
							0, true);
					}

					// bottom
					core::dimension2d<u32> leftBottomSize(0, 0);
					if (BottomLeft)
					{
						leftBottomSize = BottomLeft->getSize();
						core::rect<s32> leftRect = AbsoluteRect;
						leftRect.UpperLeftCorner.Y = leftRect.LowerRightCorner.Y - leftBottomSize.Height;
						leftRect.LowerRightCorner.X = leftRect.UpperLeftCorner.X + leftBottomSize.Width;

						driver->draw2DImage(BottomLeft,
							leftRect,
							core::rect<s32>(0, 0, leftBottomSize.Width, leftBottomSize.Height), &AbsoluteClippingRect,
							0, true);
					}

					core::dimension2d<u32> rightBottomSize(0, 0);
					if (BottomRight)
					{
						rightBottomSize = BottomRight->getSize();
						core::rect<s32> rightRect = AbsoluteRect;
						rightRect.UpperLeftCorner.Y = rightRect.LowerRightCorner.Y - rightBottomSize.Height;
						rightRect.UpperLeftCorner.X = rightRect.LowerRightCorner.X - rightBottomSize.Width;

						driver->draw2DImage(BottomRight,
							rightRect,
							core::rect<s32>(0, 0, rightBottomSize.Width, rightBottomSize.Height), &AbsoluteClippingRect,
							0, true);
					}

					if (BottomMiddle)
					{
						core::dimension2d<u32> middleSize = BottomMiddle->getSize();
						core::rect<s32> middleRect = AbsoluteRect;
						middleRect.UpperLeftCorner.X += leftSize.Width;
						middleRect.LowerRightCorner.X -= rightSize.Width;
						middleRect.UpperLeftCorner.Y = middleRect.LowerRightCorner.Y - middleSize.Height;

						driver->draw2DImage(BottomMiddle,
							middleRect,
							core::rect<s32>(0, 0, middleSize.Width, middleSize.Height), &AbsoluteClippingRect,
							0, true);
					}

					// sides
					core::dimension2d<u32> leftWinSize(0, 0);
					if (WinLeft)
					{
						leftWinSize = WinLeft->getSize();
						core::rect<s32> leftRect = AbsoluteRect;
						leftRect.LowerRightCorner.X = leftRect.UpperLeftCorner.X + leftWinSize.Width;
						leftRect.UpperLeftCorner.Y += leftSize.Height;
						leftRect.LowerRightCorner.Y -= leftBottomSize.Height;

						driver->draw2DImage(WinLeft,
							leftRect,
							core::rect<s32>(0, 0, leftWinSize.Width, leftWinSize.Height), &AbsoluteClippingRect,
							0, true);
					}

					core::dimension2d<u32> rightWinSize(0, 0);
					if (WinRight)
					{
						rightWinSize = WinRight->getSize();
						core::rect<s32> rightRect = AbsoluteRect;
						rightRect.UpperLeftCorner.X = rightRect.LowerRightCorner.X - rightWinSize.Width;
						rightRect.UpperLeftCorner.Y += rightSize.Height;
						rightRect.LowerRightCorner.Y -= rightBottomSize.Height;

						driver->draw2DImage(WinRight,
							rightRect,
							core::rect<s32>(0, 0, rightWinSize.Width, rightWinSize.Height), &AbsoluteClippingRect,
							0, true);
					}


					core::rect<s32> bodyRect = AbsoluteRect;
					bodyRect.UpperLeftCorner.X += leftWinSize.Width;
					bodyRect.UpperLeftCorner.Y += leftSize.Height;
					bodyRect.LowerRightCorner.X = bodyRect.LowerRightCorner.X - rightWinSize.Width;
					bodyRect.LowerRightCorner.Y = bodyRect.LowerRightCorner.Y - leftBottomSize.Height;

					driver->draw2DRectangle(video::SColor(255, 62, 62, 62),
						bodyRect,
						0);

					if (DrawTitlebar && Text.size())
					{
						rect = AbsoluteRect;
						rect.UpperLeftCorner.X += 20;
						rect.UpperLeftCorner.Y += 11;
						rect.LowerRightCorner.X -= 20;
						rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + 18;

						IGUIFont* font = skin->getFont(EGDF_WINDOW);
						if (font)
						{
							font->draw(Text.c_str(), rect,
								video::SColor(255, 192, 192, 192),
								false, true, &AbsoluteClippingRect);
						}
					}
				}
			}

			IGUIElement::draw();
		}


		//! Returns pointer to the close button
		IGUIButton* CGUIDecentralisedWindow::getCloseButton() const
		{
			return CloseButton;
		}


		//! Returns pointer to the minimize button
		IGUIButton* CGUIDecentralisedWindow::getMinimizeButton() const
		{
			return MinButton;
		}


		//! Returns pointer to the maximize button
		IGUIButton* CGUIDecentralisedWindow::getMaximizeButton() const
		{
			return RestoreButton;
		}


		//! Returns true if the window is draggable, false if not
		bool CGUIDecentralisedWindow::isDraggable() const
		{
			return IsDraggable;
		}


		//! Sets whether the window is draggable
		void CGUIDecentralisedWindow::setDraggable(bool draggable)
		{
			IsDraggable = draggable;

			if (Dragging && !IsDraggable)
				Dragging = false;
		}


		//! Set if the window background will be drawn
		void CGUIDecentralisedWindow::setDrawBackground(bool draw)
		{
			DrawBackground = draw;
		}


		//! Get if the window background will be drawn
		bool CGUIDecentralisedWindow::getDrawBackground() const
		{
			return DrawBackground;
		}


		//! Set if the window titlebar will be drawn
		void CGUIDecentralisedWindow::setDrawTitlebar(bool draw)
		{
			DrawTitlebar = draw;
		}


		//! Get if the window titlebar will be drawn
		bool CGUIDecentralisedWindow::getDrawTitlebar() const
		{
			return DrawTitlebar;
		}

		void CGUIDecentralisedWindow::updateClientRect()
		{
			if (!DrawBackground)
			{
				ClientRect = core::rect<s32>(0, 0, AbsoluteRect.getWidth(), AbsoluteRect.getHeight());
				return;
			}
			IGUISkin* skin = Environment->getSkin();
			skin->draw3DWindowBackground(this, DrawTitlebar,
				skin->getColor(IsActive ? EGDC_ACTIVE_BORDER : EGDC_INACTIVE_BORDER),
				AbsoluteRect, &AbsoluteClippingRect, &ClientRect);
			ClientRect -= AbsoluteRect.UpperLeftCorner;
		}


		//! Returns the rectangle of the drawable area (without border, without titlebar and without scrollbars)
		core::rect<s32> CGUIDecentralisedWindow::getClientRect() const
		{
			return ClientRect;
		}


		//! Writes attributes of the element.
		void CGUIDecentralisedWindow::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options = 0) const
		{
			CGUIDecentralisedWindow::serializeAttributes(out, options);

			out->addBool("IsDraggable", IsDraggable);
			out->addBool("DrawBackground", DrawBackground);
			out->addBool("DrawTitlebar", DrawTitlebar);

			// Currently we can't just serialize attributes of sub-elements.
			// To do this we either
			// a) allow further serialization after attribute serialiation (second function, callback or event)
			// b) add an IGUIElement attribute
			// c) extend the attribute system to allow attributes to have sub-attributes
			// We just serialize the most important info for now until we can do one of the above solutions.
			out->addBool("IsCloseVisible", CloseButton->isVisible());
			out->addBool("IsMinVisible", MinButton->isVisible());
			out->addBool("IsRestoreVisible", RestoreButton->isVisible());
		}


		//! Reads attributes of the element
		void CGUIDecentralisedWindow::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options = 0)
		{
			CGUIDecentralisedWindow::deserializeAttributes(in, options);

			Dragging = false;
			IsActive = false;
			IsDraggable = in->getAttributeAsBool("IsDraggable");
			DrawBackground = in->getAttributeAsBool("DrawBackground");
			DrawTitlebar = in->getAttributeAsBool("DrawTitlebar");

			CloseButton->setVisible(in->getAttributeAsBool("IsCloseVisible"));
			MinButton->setVisible(in->getAttributeAsBool("IsMinVisible"));
			RestoreButton->setVisible(in->getAttributeAsBool("IsRestoreVisible"));

			updateClientRect();
		}


	} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

