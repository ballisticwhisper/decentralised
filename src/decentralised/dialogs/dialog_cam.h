#ifndef __C_CAM_DIALOG_H_INCLUDED__
#define __C_CAM_DIALOG_H_INCLUDED__

#include <map>
#include "irrlicht.h"
#include "context_gui.h"
#include "world_avatar.h"
#include "IGUIDecentralisedDialog.h"
#include "CGUIDecentralisedDialog.h"
#include "CGUIDecentralisedCamControl.h"

using namespace irr;
using namespace irr::core;
using namespace video;
using namespace gui;
using namespace decentralised;
using namespace decentralised::world;

namespace decentralised
{
	namespace dialogs
	{
		class dialog_cam
		{
		public:
			~dialog_cam();

			static dialog_cam* AddDialog(context::context_gui &elems, IrrlichtDevice* device, std::map<std::wstring, std::wstring> &lang)
			{
				return new dialog_cam(elems, device, lang);
			}

			void RemoveDialog();
			void SetAvatar(world_avatar *avatar);

			bool IsOpen() {
				if (window_)
					return window_->isVisible();

				return false;
			}

		private:
			dialog_cam(context::context_gui &elems, IrrlichtDevice* device, std::map<std::wstring, std::wstring> &lang);

			void initElements();

			IrrlichtDevice* device_;
			IGUIDecentralisedDialog* window_;
			CGUIDecentralisedCamControl* camRotate_;
			CGUIDecentralisedCamControl* camTrack_;
			std::map<std::wstring, std::wstring> &lang_;
			context::context_gui &elems_;
		};
	}
}
#endif