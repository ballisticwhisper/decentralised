#include "CGUIDecentralisedCamControl.h"
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
		CGUIDecentralisedCamControl::CGUIDecentralisedCamControl(IGUIEnvironment* environment,
			IGUIElement* parent,
			s32 id,
			core::rect<s32> rectangle,
			e_cam_control_align align)
			: IGUIDecentralisedCamControl(environment, parent, id, rectangle, align), ClickTime(0), HoverTime(0), FocusTime(0)
		{
#ifdef _DEBUG
			setDebugName("CGUIDecentralisedCamControl");
#endif
			// This element can be tabbed.
			setTabStop(true);
			setTabOrder(-1);

			TrackBall = NULL;
			Arrow = NULL;
			
			isLeftPressed = false;
			isRightPressed = false;
			isUpPressed = false;
			isDownPressed = false;

			zoom = 1000;
			rotation = irr::core::vector3df(0,0,0);

			core::rect<s32> targetRect = Parent->getAbsolutePosition();
			s32 targetWidth = ((Parent->getAbsolutePosition().LowerRightCorner.X - Parent->getAbsolutePosition().UpperLeftCorner.X) / 2) - 13;
			s32 targetHeight = (Parent->getAbsolutePosition().LowerRightCorner.Y - Parent->getAbsolutePosition().UpperLeftCorner.Y) - 28;

			if (align == e_cam_control_align::Left)
			{
				targetRect.UpperLeftCorner.X += 3;
				targetRect.UpperLeftCorner.Y += 25;
				targetRect.LowerRightCorner.X = targetRect.UpperLeftCorner.X + targetWidth;
				targetRect.LowerRightCorner.Y = targetRect.UpperLeftCorner.Y + targetHeight;

				AbsoluteRect = targetRect;
				AbsoluteClippingRect = targetRect;
				RelativeRect = targetRect;

				DesiredRect = targetRect;
				DesiredRect.UpperLeftCorner.X = targetRect.UpperLeftCorner.X - Parent->getAbsolutePosition().UpperLeftCorner.X;
				DesiredRect.UpperLeftCorner.Y = targetRect.UpperLeftCorner.Y - Parent->getAbsolutePosition().UpperLeftCorner.Y;
				DesiredRect.LowerRightCorner.X = targetWidth + 3;
				DesiredRect.LowerRightCorner.Y = targetHeight + 25;
			}
			else if (align == e_cam_control_align::Right)
			{
				targetRect.UpperLeftCorner.X = targetRect.LowerRightCorner.X - targetWidth - 3;
				targetRect.UpperLeftCorner.Y += 25;
				targetRect.LowerRightCorner.X = targetRect.UpperLeftCorner.X + targetWidth;
				targetRect.LowerRightCorner.Y = targetRect.UpperLeftCorner.Y + targetHeight;

				AbsoluteRect = targetRect;
				AbsoluteClippingRect = targetRect;
				RelativeRect = targetRect;

				DesiredRect = targetRect;
				DesiredRect.UpperLeftCorner.X = targetRect.UpperLeftCorner.X - Parent->getAbsolutePosition().UpperLeftCorner.X;
				DesiredRect.UpperLeftCorner.Y = targetRect.UpperLeftCorner.Y - Parent->getAbsolutePosition().UpperLeftCorner.Y;
				DesiredRect.LowerRightCorner.X = DesiredRect.UpperLeftCorner.X + targetWidth;
				DesiredRect.LowerRightCorner.Y = targetHeight + 25;
			}
		}

		//! destructor
		CGUIDecentralisedCamControl::~CGUIDecentralisedCamControl()
		{
		}

		//! called if an event happened.
		bool CGUIDecentralisedCamControl::OnEvent(const SEvent& event)
		{
			if (!isEnabled())
				return IGUIElement::OnEvent(event);

			switch (event.EventType)
			{
			case EET_KEY_INPUT_EVENT:
				if (event.KeyInput.PressedDown &&
					(event.KeyInput.Key == KEY_RETURN || event.KeyInput.Key == KEY_SPACE))
				{

					return true;
				}
				return true;
				break;
			case EET_GUI_EVENT:
				if (event.GUIEvent.Caller == this)
				{
					if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
					{
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
					if (!AbsoluteClippingRect.isPointInside(core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y)))
					{
						isDownPressed = false;
						isUpPressed = false;
						isLeftPressed = false;
						isRightPressed = false;

						return false;
					}

					irr::core::dimension2d<s32> controlSize = AbsoluteRect.getSize();
					if (event.MouseInput.Y < AbsoluteRect.UpperLeftCorner.Y + (controlSize.Height / 2))
					{
						// Its the top half.
						if (event.MouseInput.X < AbsoluteRect.UpperLeftCorner.X + (controlSize.Width / 2))
						{
							// Its the top left quarter.
							s32 xOffset = AbsoluteRect.UpperLeftCorner.X + (controlSize.Width / 2) - event.MouseInput.X;
							s32 yOffset = AbsoluteRect.UpperLeftCorner.Y + (controlSize.Height / 2) - event.MouseInput.Y;
							if (xOffset > yOffset)
								isLeftPressed = true;
							else
								isUpPressed = true;
						}
						else if (event.MouseInput.X >= AbsoluteRect.UpperLeftCorner.X + (controlSize.Width / 2))
						{
							// Its the top right quarter.
							s32 xOffset = event.MouseInput.X - AbsoluteRect.UpperLeftCorner.X + (controlSize.Width / 2) - controlSize.Width;
							s32 yOffset = AbsoluteRect.UpperLeftCorner.Y + (controlSize.Height / 2) - event.MouseInput.Y;
							if (xOffset > yOffset)
								isRightPressed = true;
							else
								isUpPressed = true;
						}
						isUpPressed = true;
					}
					else if (event.MouseInput.Y >= AbsoluteRect.UpperLeftCorner.Y + (controlSize.Height / 2))
					{
						// Its the bottom half.
						if (event.MouseInput.X < AbsoluteRect.UpperLeftCorner.X + (controlSize.Width / 2))
						{
							// Its the bottom left quarter.
							s32 xOffset = AbsoluteRect.UpperLeftCorner.X + (controlSize.Width / 2) - event.MouseInput.X;
							s32 yOffset = event.MouseInput.Y - AbsoluteRect.UpperLeftCorner.Y + (controlSize.Height / 2) - controlSize.Height;
							if (xOffset > yOffset)
								isLeftPressed = true;
							else
								isDownPressed = true;
						}
						else if (event.MouseInput.X >= AbsoluteRect.UpperLeftCorner.X + (controlSize.Width / 2))
						{
							// Its the bottom right quarter.
							s32 xOffset = event.MouseInput.X - AbsoluteRect.UpperLeftCorner.X + (controlSize.Width / 2) - controlSize.Width;
							s32 yOffset = event.MouseInput.Y - AbsoluteRect.UpperLeftCorner.Y + (controlSize.Height / 2) - controlSize.Height;
							if (xOffset > yOffset)
								isRightPressed = true;
							else
								isDownPressed = true;
						}
						isDownPressed = true;
					}

					return true;
				}
				else
					if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
					{
						isDownPressed = false;
						isUpPressed = false;
						isLeftPressed = false;
						isRightPressed = false;

						if (!AbsoluteClippingRect.isPointInside(core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y)))
						{
							return true;
						}

						//if ((!IsPushButton && wasPressed && Parent) ||
						//	(IsPushButton && wasPressed != Pressed))
						//{
						//	SEvent newEvent;
						//	newEvent.EventType = EET_GUI_EVENT;
						//	newEvent.GUIEvent.Caller = this;
						//	newEvent.GUIEvent.Element = 0;
						//	newEvent.GUIEvent.EventType = EGET_BUTTON_CLICKED;
						//	Parent->OnEvent(newEvent);
						//}

						return true;
					}
				break;
			default:
				break;
			}

			return Parent ? Parent->OnEvent(event) : false;
		}


		//! draws the element and its children
		void CGUIDecentralisedCamControl::draw()
		{
			if (!IsVisible)
				return;

			IGUISkin* skin = Environment->getSkin();
			video::IVideoDriver* driver = Environment->getVideoDriver();

			if (TrackBall)
			{
				core::dimension2d<u32> sourceSize = TrackBall->getSize();
				core::rect<s32> targetRect = AbsoluteRect;

				driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
				driver->getMaterial2D().AntiAliasing = video::EAAM_QUALITY;

				driver->enableMaterial2D();
				driver->draw2DImage(TrackBall,
					targetRect,
					core::rect<s32>(0, 0, sourceSize.Width, sourceSize.Height), &targetRect,
					0, true);
				driver->enableMaterial2D(false);
			}

			if (Arrow && (isUpPressed || isDownPressed || isLeftPressed || isRightPressed))
			{
				core::dimension2d<u32> sourceSize = Arrow->getSize();
				core::rect<s32> targetRect = AbsoluteRect;

				driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
				driver->getMaterial2D().AntiAliasing = video::EAAM_QUALITY;

				s32 arrowWidth = sourceSize.Width / 2;
				s32 arrowHeight = sourceSize.Height / 2;

				core::rect<s32> srcRect(0, 0, arrowWidth, arrowHeight);
				irr::core::vector3df pos = camera_->getPosition();
				irr::core::vector3df tar = camera_->getTarget();
				if (isLeftPressed)
				{
					srcRect = core::rect<s32>(0, arrowHeight, arrowWidth, arrowHeight * 2);
					if (camera_)
					{
						rotation.X -= 0.001;
						if (rotation.X < 0)
							rotation.X = 6.28219;
					}
				}
				else if (isRightPressed)
				{
					srcRect = core::rect<s32>(arrowWidth, arrowHeight, arrowWidth * 2, arrowHeight * 2);
					if (camera_)
					{
						rotation.X += 0.001;
						if (rotation.X >= 6.28319)
							rotation.X = 0;
					}
				}
				else if (isDownPressed)
				{
					srcRect = core::rect<s32>(arrowWidth, 0, arrowWidth * 2, arrowHeight);
					if (camera_)
					{
						rotation.Y += 0.001;
						if (rotation.Y > 3.139595)
							rotation.Y = 3.139595;
					}
				}
				else if (isUpPressed)
				{
					if (camera_)
					{
						rotation.Y -= 0.001;
						if (rotation.Y < 0.002)
							rotation.Y = 0.002;
					}
				}

				pos.X = tar.X + (zoom * sin(rotation.Y) * cos(rotation.X));
				pos.Y = tar.Y + (zoom * cos(rotation.Y));
				pos.Z = tar.Z + (zoom * sin(rotation.Y) * sin(rotation.X));

				camera_->setPosition(pos);

				driver->enableMaterial2D();
				driver->draw2DImage(Arrow,
					targetRect,
					srcRect,
					&targetRect,
					0, true);
				driver->enableMaterial2D(false);
			}

			IGUIElement::draw();
		}

		void CGUIDecentralisedCamControl::setImages(video::ITexture* trackBall, video::ITexture* arrow)
		{
			TrackBall = trackBall;
			Arrow = arrow;
		}

		void CGUIDecentralisedCamControl::setCamera(irr::scene::ICameraSceneNode* camera)
		{
			camera_ = camera;
			rotation = camera_->getRotation();
		}
	}
}

#endif

