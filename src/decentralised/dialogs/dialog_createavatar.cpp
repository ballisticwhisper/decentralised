#include "dialog_createavatar.h"
#include "manager_gui.h"
using namespace decentralised::managers;

namespace decentralised
{
	namespace dialogs
	{
		dialog_createavatar::dialog_createavatar(context::context_gui &elems, IGUIEnvironment* env, std::map<std::wstring, std::wstring> &lang)
			: env_(env),
			lang_(lang),
			elems_(elems)
		{
			initElements();
		}

		dialog_createavatar::~dialog_createavatar()
		{
		}

		void dialog_createavatar::initElements()
		{
			stringw title = stringw(lang_[L"CreateAvatar_WindowTitle"].c_str());

			window_ = new CGUIDecentralisedWindow(env_,
				env_->getRootGUIElement(),
				e_gui_elements::WindowCreateAvatar,
				rect<s32>(80, 40, 360, 330),
				elems_.TxWindowTitleLeft,
				elems_.TxWindowTitleMiddle,
				elems_.TxWindowTitleRight,
				elems_.TxWindowLeft,
				elems_.TxWindowRight,
				elems_.TxWindowBottomLeft,
				elems_.TxWindowBottomMiddle,
				elems_.TxWindowBottomRight);
			window_->setVisible(true);
			window_->setText(title.c_str());
			env_->setFocus(window_);
			window_->drop();

			s32 posy = 40;

			elems_.CreateAvFirstNameLabel = env_->addStaticText(lang_[L"CreateAvatar_FirstnameLabel"].c_str(),
				rect<s32>(20, posy, 140, posy + 22),
				false, true,
				window_,
				0);

			elems_.CreateAvLastNameLabel = env_->addStaticText(lang_[L"CreateAvatar_LastnameLabel"].c_str(),
				rect<s32>(140, posy, 310, posy + 22),
				false, true,
				window_,
				0);

			posy += 22;

			elems_.CreateAvFirstNameTextBox = new CGUIDecentralisedTextbox(L"", true, env_, window_, e_gui_elements::CreateAvFirstnameBox, rect<s32>(20, posy, 135, posy + 22));
			elems_.CreateAvFirstNameTextBox->setEnabled(true);
			elems_.CreateAvFirstNameTextBox->setOverrideColor(SColor(255, 0, 0, 0));
			elems_.CreateAvFirstNameTextBox->drop();

			elems_.CreateAvLastNameTextBox = new CGUIDecentralisedTextbox(L"", true, env_, window_, e_gui_elements::CreateAvLastnameBox, rect<s32>(140, posy, 255, posy + 22));
			elems_.CreateAvLastNameTextBox->setEnabled(true);
			elems_.CreateAvLastNameTextBox->setOverrideColor(SColor(255, 0, 0, 0));
			elems_.CreateAvLastNameTextBox->drop();

			posy += 32;

			elems_.CreateAvPasswordLabel = env_->addStaticText(lang_[L"CreateAvatar_PasswordLabel"].c_str(),
				rect<s32>(20, posy, 260, posy + 22),
				false, true,
				window_,
				0);

			posy += 22;

			elems_.CreateAvPasswordBox = new CGUIDecentralisedTextbox(L"", true, env_, window_, e_gui_elements::CreateAvPasswordBox, rect<s32>(20, posy, 255, posy + 22));
			elems_.CreateAvPasswordBox->setEnabled(true);
			elems_.CreateAvPasswordBox->setOverrideColor(SColor(255, 0, 0, 0));
			elems_.CreateAvPasswordBox->setPasswordBox(true, L'\x25cf');
			elems_.CreateAvPasswordBox->drop();

			posy += 32;

			elems_.CreateAvConfirmLabel = env_->addStaticText(lang_[L"CreateAvatar_ConfirmLabel"].c_str(),
				rect<s32>(20, posy, 260, posy + 22),
				false, true,
				window_,
				0);

			posy += 22;

			elems_.CreateAvConfirmPasswordBox = new CGUIDecentralisedTextbox(L"", true, env_, window_, e_gui_elements::CreateAvConfirmBox, rect<s32>(20, posy, 255, posy + 22));
			elems_.CreateAvConfirmPasswordBox->setEnabled(true);
			elems_.CreateAvConfirmPasswordBox->setOverrideColor(SColor(255, 0, 0, 0));
			elems_.CreateAvConfirmPasswordBox->setPasswordBox(true, L'\x25cf');
			elems_.CreateAvConfirmPasswordBox->drop();

			posy += 32;

			elems_.CreateAvWarningLabel = env_->addStaticText(lang_[L"CreateAvatar_Warning"].c_str(),
				rect<s32>(20, posy, 260, posy + 42),
				false, true,
				window_,
				e_gui_elements::CreateAvWarningLabel);
			elems_.CreateAvWarningLabel->setOverrideColor(SColor(255, 181, 66, 66));

			posy += 42;

			//elems_.CreateAvCreateButton = env_->addVoodooButton(rect<s32>(20, posy, 100, posy + 25),
			//	window_,
			//	e_gui_elements::CreateAvCreateButton,
			//	lang_[L"CreateAvatar_CreateButtonText"].c_str(),
			//	elems_.TxButtonLeft,
			//	elems_.TxButtonMiddle,
			//	elems_.TxButtonRight,
			//	elems_.TxButtonPressedLeft,
			//	elems_.TxButtonPressedMiddle,
			//	elems_.TxButtonPressedRight);

			if (elems_.LoginButton)
				elems_.LoginButton->setEnabled(true);
		}

		void dialog_createavatar::RemoveDialog()
		{
			window_->remove();
		}
	}
}