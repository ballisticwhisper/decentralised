#ifndef __I_GUI_DECENTRALISED_CAM_CONTROL_H_INCLUDED__
#define __I_GUI_DECENTRALISED_CAM_CONTROL_H_INCLUDED__

#include "IGUIElement.h"
#include "EMessageBoxFlags.h"

enum e_cam_control_align {
	Left = 0,
	Right = 1
};

namespace irr
{
namespace gui
{
	class IGUIDecentralisedCamControl : public IGUIElement
	{
	public:

		//! constructor
		IGUIDecentralisedCamControl(IGUIEnvironment* environment,
			IGUIElement* parent,
			s32 id,
			core::rect<s32> rectangle,
			e_cam_control_align align)
			: IGUIElement(EGUIET_BUTTON, environment, parent, id, rectangle) {}

	};


}
}

#endif

