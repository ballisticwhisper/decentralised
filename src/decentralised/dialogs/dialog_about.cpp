#include "dialog_about.h"
#include "manager_gui.h"
using namespace decentralised::managers;

namespace decentralised
{
	namespace dialogs
	{
		dialog_about::dialog_about(context::context_gui &elems, IGUIEnvironment* env, std::map<std::wstring, std::wstring> &lang)
			: env_(env),
			lang_(lang),
			elems_(elems)
		{
			initElements();
		}

		dialog_about::~dialog_about()
		{
		}

		void dialog_about::initElements()
		{
			stringw title = stringw(lang_[L"About_WindowTitle"].c_str()).replace("{0}", APP_TITLE);

			window_ = new CGUIDecentralisedWindow(env_,
				0,
				e_gui_elements::WindowAbout,
				rect<s32>(80, 40, 420, 290),
				elems_.TxWindowTitleLeft,
				elems_.TxWindowTitleMiddle,
				elems_.TxWindowTitleRight,
				elems_.TxWindowLeft,
				elems_.TxWindowRight,
				elems_.TxWindowBottomLeft,
				elems_.TxWindowBottomMiddle,
				elems_.TxWindowBottomRight);
			env_->setFocus(window_);
			window_->drop();

			s32 posy = 60;

			env_->addStaticText(APP_TITLE,
				rect<s32>(20, posy, 320, posy + 22),
				false, true,
				window_);

			posy += 22;

			//IGUIButton* linkBtn = env_->addButton(rect<s32>(20, posy, 198, posy + 22), 
			//	window_,
			//	e_gui_elements::AboutLink,
			//	L"http://decentralised-project.org");
			//linkBtn->setOverrideTextColor(SColor(255, 101, 174, 235));

			posy += 34;

			stringw lead = stringw(lang_[L"About_LeadIntro"].c_str()).replace("{0}", APP_TITLE);
			env_->addStaticText(lead.c_str(),
				rect<s32>(20, posy, 320, posy + 44),
				false, true,
				window_);

			posy += 44;

			env_->addStaticText(lang_[L"About_Credit1"].c_str(),
				rect<s32>(20, posy, 320, posy + 22),
				false, true,
				window_);

			posy += 22;

			env_->addStaticText(lang_[L"About_Credit2"].c_str(),
				rect<s32>(20, posy, 320, posy + 22),
				false, true,
				window_);

			posy += 22;

			env_->addStaticText(lang_[L"About_Credit3"].c_str(),
				rect<s32>(20, posy, 320, posy + 22),
				false, true,
				window_);

			posy += 22;

		}

		void dialog_about::RemoveDialog()
		{
			window_->remove();
		}
	}
}