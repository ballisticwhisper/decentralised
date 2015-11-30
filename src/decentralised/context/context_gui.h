#ifndef __CONTEXT_GUI_H_INCLUDED__
#define __CONTEXT_GUI_H_INCLUDED__

#include "irrlicht.h"
#include "IGUIConsoleOverlay.h"
#include "CGUIDecentralisedButton.h"
#include "CGUIDecentralisedContextMenu.h"
#include "CGUIDecentralisedTextbox.h"

using namespace irr;
using namespace gui;
using namespace video;

namespace decentralised
{
	namespace context
	{
		class context_gui
		{
		public:
			context_gui() {}
			~context_gui() {}

			IGUIConsoleOverlay* Console;

			IGUIToolBar* BottomBar;
			IGUIStaticText* LoginLabel;
			IGUIStaticText* AvatarNameLabel;
			IGUIStaticText* PasswordLabel;
			IGUIComboBox* AvatarCombo;
			IGUIEditBox* PasswordBox;
			CGUIDecentralisedButton* LoginButton;
			CGUIDecentralisedButton* CreateAvButton;

			IGUIStaticText* CreateAvFirstNameLabel;
			CGUIDecentralisedTextbox* CreateAvFirstNameTextBox;
			IGUIStaticText* CreateAvLastNameLabel;
			CGUIDecentralisedTextbox* CreateAvLastNameTextBox;
			IGUIStaticText* CreateAvPasswordLabel;
			CGUIDecentralisedTextbox* CreateAvPasswordBox;
			IGUIStaticText* CreateAvConfirmLabel;
			CGUIDecentralisedTextbox* CreateAvConfirmPasswordBox;
			IGUIStaticText* CreateAvWarningLabel;
			CGUIDecentralisedButton* CreateAvCreateButton;

			CGUIDecentralisedContextMenu* MenuBarMain;
			IGUIContextMenu* MenuTime;

			ITexture* TxButtonLeft;
			ITexture* TxButtonMiddle;
			ITexture* TxButtonRight;

			ITexture* TxButtonPressedLeft;
			ITexture* TxButtonPressedMiddle;
			ITexture* TxButtonPressedRight;

			ITexture* TxWindowTitleLeft;
			ITexture* TxWindowTitleMiddle;
			ITexture* TxWindowTitleRight;

			ITexture* TxWindowLeft;
			ITexture* TxWindowRight;

			ITexture* TxWindowBottomLeft;
			ITexture* TxWindowBottomMiddle;
			ITexture* TxWindowBottomRight;

			ITexture* TxConsoleTopLeft;
			ITexture* TxConsoleTopRight;
			ITexture* TxConsoleBottomLeft;
			ITexture* TxConsoleBottomRight;
		};
	}
}

#endif