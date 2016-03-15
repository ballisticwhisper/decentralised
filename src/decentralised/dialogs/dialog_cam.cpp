#include "dialog_cam.h"
#include "manager_gui.h"
using namespace decentralised::managers;

namespace decentralised
{
	namespace dialogs
	{
		dialog_cam::dialog_cam(context::context_gui &elems, IrrlichtDevice* device, std::map<std::wstring, std::wstring> &lang)
			: device_(device),
			lang_(lang),
			elems_(elems)
		{
			initElements();
		}

		dialog_cam::~dialog_cam()
		{
		}

		void dialog_cam::initElements()
		{
			stringw title = stringw(lang_[L"About_WindowTitle"].c_str()).replace("{0}", APP_TITLE);

			IGUIEnvironment *env = device_->getGUIEnvironment();

			window_ = new CGUIDecentralisedDialog(env,
				env->getRootGUIElement(),
				e_gui_elements::WindowAbout,
				rect<s32>(100, 100, 253, 182),
				false);
			window_->setMaxSize(dimension2du(800, 600));
			window_->setDialogSkin(elems_.TxDialogBackNoTitle, elems_.TxDialogForeNoTitle);
			window_->setText(title.c_str());
			env->setFocus(window_);
			window_->drop();

			camRotate_ = new CGUIDecentralisedCamControl(env, window_, -1, irr::core::rect<s32>(0,0,10,10), e_cam_control_align::Left);
			camRotate_->setImages(elems_.TxCamRotate, elems_.TxCamRotateArrow);
			camRotate_->drop();

			camTrack_ = new CGUIDecentralisedCamControl(env, window_, -1, irr::core::rect<s32>(50, 0, 60, 10), e_cam_control_align::Right);
			camTrack_->setImages(elems_.TxCamTracking, elems_.TxCamTrackingArrow);
			camTrack_->drop();
		}

		void dialog_cam::RemoveDialog()
		{
			window_->remove();
		}

		void dialog_cam::SetAvatar(world_avatar *avatar)
		{
			camRotate_->setCamera(avatar->getCamera());
			camTrack_->setCamera(avatar->getCamera());
		}
	}
}