#ifndef __MANAGER_GUI_H_INCLUDED__
#define __MANAGER_GUI_H_INCLUDED__

#include <irrlicht.h>
#include "CGUIDecentralisedButton.h"
#include "IGUIDecentralisedWindow.h"
#include "IGUIConsoleOverlay.h"
#include "CGUIConsoleOverlay.h"
#include "CGUIDecentralisedSkin.h"
#include "CGUIDecentralisedMenuBar.h"
#include "CGUIDecentralisedContextMenu.h"
#include "CGUIDecentralisedTextbox.h"
#include "CGUIDecentralisedDropdown.h"
#include "CGUIDecentralisedToolbar.h"
#include "context_gui.h"
#include "context_dialogs.h"
#include "dc-config.h"
#include <ctime>
#include <vector>
#include <map>

using namespace irr;

using namespace irr::core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace decentralised;

namespace decentralised
{
	namespace managers
	{
		enum e_modals
		{
			Created = 0,
			PasswordsDoNotMatch,
			AllFieldsMustBeCompleted,
		};

		enum e_menu_items
		{
			ExitApplication = 0,
			Preferences,
			AboutApplication,
			ReportBug,
			Wiki
		};

		enum e_gui_elements
		{
			LoginBackground = 0,
			LoginToolbar,
			LoginToolbarAvatarCombo,
			LoginToolbarPasswordBox,
			LoginToolbarLoginButton,
			LoginToolbarCreateAvatarButton,
			ErrorModal,
			ErrorModalOkButton,

			WindowCreateAvatar,
			CreateAvFirstnameBox,
			CreateAvLastnameBox,
			CreateAvPasswordBox,
			CreateAvConfirmBox,
			CreateAvWarningLabel,
			CreateAvCreateButton,

			WindowAbout,
			AboutLink,
		};

		class manager_gui
		{
		public:
			manager_gui(IrrlichtDevice* device,
						std::map<std::wstring, std::wstring> &language, 
						std::map<std::wstring, std::wstring> &skin,
						std::map<std::wstring, std::wstring> &config);
			~manager_gui();

			virtual void Initialize();
			virtual void DrawAll();
			virtual stringw GetElementValue(s32 id);
			virtual void ShowModal(s32 id);
			virtual void AddConsoleLine(video::SColor textColor, std::wstring text);
			virtual void SetCurrentTime(time_t currentTime);

			virtual void ToggleWindowCreateAvatar();
			virtual void CleanupWindowCreateAvatar();

			virtual void ShowWindowAbout();
			virtual void CleanupWindowAbout();

			IGUIImage* Background;

		private:
			IGUIEnvironment* env;
			IrrlichtDevice* dev;
			std::map<std::wstring, std::wstring> &lang_;
			std::map<std::wstring, std::wstring> &skin_;
			std::map<std::wstring, std::wstring> &config_;

			context::context_gui elems;
			context::context_dialogs dialogs;

			virtual video::ITexture* loadTexture(IVideoDriver* driver, stringw skinPath, stringw filename);
		};
	}
}

#endif