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
				env_->getRootGUIElement(),
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
			window_->setText(title.c_str());
			env_->setFocus(window_);
			window_->drop();

			s32 posy = 60;

			env_->addStaticText(APP_TITLE,
				rect<s32>(20, posy, 320, posy + 22),
				false, true,
				window_);

			posy += 22;

			CGUIDecentralisedButton* linkBtn = new CGUIDecentralisedButton(env_, window_, e_gui_elements::AboutLink, rect<s32>(20, posy, 198, posy + 22));
			linkBtn->setText(L"http://decentralised-project.org");
			linkBtn->setButtonTextAlign(e_button_text_align::Left);
			linkBtn->setOverrideTextColor(SColor(255, 101, 174, 235));
			linkBtn->drop();

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