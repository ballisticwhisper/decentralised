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
#include "world_avatar.h"
#include "dc-config.h"
#include <ctime>
#include <vector>
#include <map>
#include "core/utility/elliptic_curve_key.hpp"

using namespace irr;

using namespace irr::core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace decentralised;
using namespace decentralised::world;

namespace decentralised
{
	namespace managers
	{
		enum e_state
		{
			Login = 0,
			NavigateWorld
		};

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

			void Initialize();
			void DrawAll();
			stringw GetElementValue(s32 id);
			void ShowModal(s32 id);
			void AddConsoleLine(video::SColor textColor, std::wstring text);
			void SetCurrentTime(time_t currentTime);
			void SetOwnAvatars(std::vector<avatar_file>* avatars);
			void SetState(e_state state);
			void SetAvatar(world_avatar* avatar);

			void ToggleWindowCreateAvatar(elliptic_curve_key &keyPair);
			void CleanupWindowCreateAvatar();

			void ShowWindowAbout();
			void CleanupWindowAbout();

			IGUIImage* Background;

		private:
			e_state state_;
			IGUIEnvironment* env;
			IrrlichtDevice* dev;
			std::map<std::wstring, std::wstring> &lang_;
			std::map<std::wstring, std::wstring> &skin_;
			std::map<std::wstring, std::wstring> &config_;

			context::context_gui elems;
			context::context_dialogs dialogs;
			world_avatar *avatar_;

			void initializeMenuBar();
			void initializeConsole();
			video::ITexture* loadTexture(IVideoDriver* driver, stringw skinPath, stringw filename);
		};
	}
}

#endif