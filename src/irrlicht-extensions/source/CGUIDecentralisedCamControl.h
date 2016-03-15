#ifndef __C_GUI_DECENTRALISED_CAM_CONTROL_H_INCLUDED__
#define __C_GUI_DECENTRALISED_CAM_CONTROL_H_INCLUDED__

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUIButton.h"
#include "IGUIDecentralisedCamControl.h"
#include "IGUISpriteBank.h"
#include "ICameraSceneNode.h"
#include "SColor.h"

namespace irr
{
	namespace gui
	{
		class CGUIDecentralisedCamControl : public IGUIDecentralisedCamControl
		{
		public:

			//! constructor
			CGUIDecentralisedCamControl(IGUIEnvironment* environment,
										IGUIElement* parent,
										s32 id,
										core::rect<s32> rectangle,
										e_cam_control_align align);

			//! destructor
			virtual ~CGUIDecentralisedCamControl();

			//! called if an event happened.
			virtual bool OnEvent(const SEvent& event);

			//! draws the element and its children
			virtual void draw();

			virtual void setImages(video::ITexture* trackBall = 0, video::ITexture* arrow = 0);

			virtual void setCamera(irr::scene::ICameraSceneNode* camera);

		private:

			video::ITexture* TrackBall;
			video::ITexture* Arrow;

			core::rect<s32> ImageRect;
			core::rect<s32> PressedImageRect;

			bool isLeftPressed;
			bool isUpPressed;
			bool isRightPressed;
			bool isDownPressed;

			s32 zoom;
			irr::core::vector3df rotation;

			u32 ClickTime, HoverTime, FocusTime;
			irr::scene::ICameraSceneNode* camera_;
		};

	}
}

#endif
#endif

