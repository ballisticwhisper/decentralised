#include "dialog_createavatar.h"
#include "manager_gui.h"
using namespace decentralised::managers;

namespace decentralised
{
	namespace dialogs
	{
		dialog_createavatar::dialog_createavatar(context::context_gui &elems, IGUIEnvironment* env, std::map<std::wstring, std::wstring> &lang, elliptic_curve_key &keyPair)
			: env_(env),
			lang_(lang),
			elems_(elems),
			keyPair_(keyPair)
		{
		}

		dialog_createavatar::~dialog_createavatar()
		{
			elems_.CreateAvPublicKeyBox->drop();
		}

		void dialog_createavatar::InitElements()
		{
			stringw title = stringw(lang_[L"CreateAvatar_WindowTitle"].c_str());

			window_ = new CGUIDecentralisedDialog(env_,
				env_->getRootGUIElement(),
				e_gui_elements::WindowCreateAvatar,
				rect<s32>(80, 40, 400, 250),
				true);
			window_->setDialogSkin(elems_.TxDialogBack, elems_.TxDialogFore);
			window_->setVisible(true);
			window_->setText(title.c_str());
			env_->setFocus(window_);
			window_->drop();

			s32 posy = 40;

			elems_.CreateAvPublicKeyLabel = env_->addStaticText(lang_[L"CreateAvatar_PublicKeyLabel"].c_str(),
				rect<s32>(20, posy, 260, posy + 22),
				false, true,
				window_,
				0);

			posy += 22;

			elems_.CreateAvPublicKeyBox = new CGUIDecentralisedTextbox(L"", true, env_, window_, e_gui_elements::CreateAvPasswordBox, rect<s32>(20, posy, 295, posy + 22));
			elems_.CreateAvPublicKeyBox->setEnabled(false);
			elems_.CreateAvPublicKeyBox->setOverrideColor(SColor(255, 0, 0, 0));

			posy += 32;

			elems_.CreateAvFirstNameLabel = env_->addStaticText(lang_[L"CreateAvatar_FirstnameLabel"].c_str(),
				rect<s32>(20, posy, 140, posy + 22),
				false, true,
				window_,
				0);

			elems_.CreateAvLastNameLabel = env_->addStaticText(lang_[L"CreateAvatar_LastnameLabel"].c_str(),
				rect<s32>(160, posy, 310, posy + 22),
				false, true,
				window_,
				0);

			posy += 22;

			elems_.CreateAvFirstNameTextBox = new CGUIDecentralisedTextbox(L"", true, env_, window_, e_gui_elements::CreateAvFirstnameBox, rect<s32>(20, posy, 155, posy + 22));
			elems_.CreateAvFirstNameTextBox->setEnabled(true);
			elems_.CreateAvFirstNameTextBox->setOverrideColor(SColor(255, 0, 0, 0));
			elems_.CreateAvFirstNameTextBox->drop();

			elems_.CreateAvLastNameTextBox = new CGUIDecentralisedTextbox(L"", true, env_, window_, e_gui_elements::CreateAvLastnameBox, rect<s32>(160, posy, 295, posy + 22));
			elems_.CreateAvLastNameTextBox->setEnabled(true);
			elems_.CreateAvLastNameTextBox->setOverrideColor(SColor(255, 0, 0, 0));
			elems_.CreateAvLastNameTextBox->drop();

			posy += 40;

			elems_.CreateAvCreateButton = new CGUIDecentralisedButton(env_, window_, e_gui_elements::CreateAvCreateButton, rect<s32>(20, posy, 180, posy + 25));
			elems_.CreateAvCreateButton->setImages(elems_.TxButtonLeft, elems_.TxButtonMiddle, elems_.TxButtonRight, elems_.TxButtonPressedLeft, elems_.TxButtonPressedMiddle, elems_.TxButtonPressedRight);
			elems_.CreateAvCreateButton->setText(lang_[L"CreateAvatar_CreateButtonText"].c_str());
			elems_.CreateAvCreateButton->drop();

			if (elems_.LoginButton)
				elems_.LoginButton->setEnabled(true);

			data_chunk publicKeyChunk = keyPair_.public_key();
			payment_address address;
			set_public_key(address, keyPair_.public_key());

			std::string encoded = address.encoded();
			std::wstring publicKey(encoded.begin(), encoded.end());
			elems_.CreateAvPublicKeyBox->setText(publicKey.c_str());
		}

		void dialog_createavatar::RemoveDialog()
		{
			window_->remove();
		}
	}
}