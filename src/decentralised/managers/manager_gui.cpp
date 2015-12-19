#include "manager_gui.h"

namespace decentralised
{
	namespace managers
	{
		manager_gui::manager_gui(IrrlichtDevice* device,
			std::map<std::wstring, std::wstring> &language,
			std::map<std::wstring, std::wstring> &skin,
			std::map<std::wstring, std::wstring> &config)
			: lang_(language),
			skin_(skin),
			config_(config)
		{
			env = device->getGUIEnvironment();
			dev = device;

			dialogs.CreateAvatar = NULL;
			dialogs.About = NULL;
		}

		manager_gui::~manager_gui()
		{
			if (dialogs.CreateAvatar)
				delete dialogs.CreateAvatar;

			if (dialogs.About)
				delete dialogs.About;
		}

		void manager_gui::Initialize()
		{
			stringw skinPath = "data/Skins/";
			skinPath.append(config_[L"Skin"].c_str());
			skinPath.append("/");

			CGUIDecentralisedSkin* skin = new CGUIDecentralisedSkin(dev->getVideoDriver(), skin_, skinPath.c_str());
			IGUIFont* builtinfont = env->getBuiltInFont();
			IGUIFontBitmap* bitfont = 0;
			if (builtinfont && builtinfont->getType() == EGFT_BITMAP)
				bitfont = (IGUIFontBitmap*)builtinfont;

			IGUISpriteBank* bank = 0;
			skin->setFont(builtinfont);

			if (bitfont)
				bank = bitfont->getSpriteBank();

			skin->setSpriteBank(bank);

			env->setSkin(skin);
			skin->drop();

			stringw standardFontPath = skinPath.c_str();
			standardFontPath.append(skin_[L"General_StandardFont"].c_str());
			

			IGUIFont* font = env->getFont(standardFontPath);
			font->setKerningWidth(0);
			if (font)
				skin->setFont(font);

			stringw boldFontPath = skinPath.c_str();
			boldFontPath.append(skin_[L"General_BoldFont"].c_str());

			IGUIFont* boldFont = env->getFont(boldFontPath);

			IVideoDriver* driver = dev->getVideoDriver();
			IFileSystem *fs = dev->getFileSystem();

			rect<s32> winSize = env->getRootGUIElement()->getAbsoluteClippingRect();

			elems.TxButtonLeft = loadTexture(driver, skinPath, skin_[L"Button_BackImageLeft"].c_str());
			elems.TxButtonMiddle = loadTexture(driver, skinPath, skin_[L"Button_BackImageMiddle"].c_str());
			elems.TxButtonRight = loadTexture(driver, skinPath, skin_[L"Button_BackImageRight"].c_str());
			elems.TxButtonPressedLeft = loadTexture(driver, skinPath, skin_[L"Button_PressedImageLeft"].c_str());
			elems.TxButtonPressedMiddle = loadTexture(driver, skinPath, skin_[L"Button_PressedImageMiddle"].c_str());
			elems.TxButtonPressedRight = loadTexture(driver, skinPath, skin_[L"Button_PressedImageRight"].c_str());

			elems.TxWindowTitleLeft = loadTexture(driver, skinPath, skin_[L"Window_TitleBarImageLeft"].c_str());
			elems.TxWindowTitleMiddle = loadTexture(driver, skinPath, skin_[L"Window_TitleBarImageMiddle"].c_str());
			elems.TxWindowTitleRight = loadTexture(driver, skinPath, skin_[L"Window_TitleBarImageRight"].c_str());
			elems.TxWindowLeft = loadTexture(driver, skinPath, skin_[L"Window_ImageLeft"].c_str());
			elems.TxWindowRight = loadTexture(driver, skinPath, skin_[L"Window_ImageRight"].c_str());
			elems.TxWindowBottomLeft = loadTexture(driver, skinPath, skin_[L"Window_BottomImageLeft"].c_str());
			elems.TxWindowBottomMiddle = loadTexture(driver, skinPath, skin_[L"Window_BottomImageMiddle"].c_str());
			elems.TxWindowBottomRight = loadTexture(driver, skinPath, skin_[L"Window_BottomImageRight"].c_str());

			elems.TxConsoleTopLeft = loadTexture(driver, skinPath, skin_[L"Console_CornerTopLeft"].c_str());
			elems.TxConsoleTopRight = loadTexture(driver, skinPath, skin_[L"Console_CornerTopRight"].c_str());
			elems.TxConsoleBottomLeft = loadTexture(driver, skinPath, skin_[L"Console_CornerBottomLeft"].c_str());
			elems.TxConsoleBottomRight = loadTexture(driver, skinPath, skin_[L"Console_CornerBottomRight"].c_str());

			Background = env->addImage(rect<s32>(0, 0, winSize.LowerRightCorner.X, winSize.LowerRightCorner.Y),
				env->getRootGUIElement(),
				e_gui_elements::LoginBackground, 0, false);
			Background->setScaleImage(true);
			Background->setAlignment(EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE, EGUIA_SCALE);
			Background->setMinSize(dimension2du(800, 600));

			stringw backgroundPath = skinPath.c_str();
			backgroundPath.append(skin_[L"General_BackgroundImage"].c_str());
			Background->setImage(driver->getTexture(backgroundPath));
			
			IGUIElement* root = env->getRootGUIElement();

			elems.Console = new CGUIConsoleOverlay(env, root, -1, rect<s32>(20, 0, 500, 490),
				elems.TxConsoleTopLeft,
				elems.TxConsoleTopRight,
				elems.TxConsoleBottomLeft,
				elems.TxConsoleBottomRight);
			elems.Console->drop();

			elems.MenuBarMain = new CGUIDecentralisedMenuBar(env, root, -1, rect<s32>(0, 0, root->getAbsolutePosition().getWidth(), root->getAbsolutePosition().getHeight()));

			elems.MenuBarMain->addItem(lang_[L"Menus_AvatarMenu"].c_str(), -1, true, true, false, true);
			gui::IGUIContextMenu* submenu;
			submenu = elems.MenuBarMain->getSubMenu(0);
			submenu->addItem(lang_[L"Menus_PreferencesItem"].c_str(), e_menu_items::Preferences);
			submenu->addSeparator();
			submenu->addItem(stringw(lang_[L"Menus_ExitItem"].c_str()).replace(stringw("{0}"), APP_TITLE).c_str(), e_menu_items::ExitApplication);

			//elems.MenuBarMain->addItem(lang_[L"Menus_WorldMenu"].c_str(), -1, true, true, false, false);
			//submenu = elems.MenuBarMain->getSubMenu(1);
			//submenu->addItem(stringw(lang_[L"Menus_MineItem"].c_str()).replace(stringw("{0}"), APP_TITLE).c_str(), e_menu_items::Wiki);

			elems.MenuBarMain->addItem(lang_[L"Menus_HelpMenu"].c_str(), -1, true, true, false, false);
			submenu = elems.MenuBarMain->getSubMenu(1);
			submenu->addItem(stringw(lang_[L"Menus_WikiItem"].c_str()).replace(stringw("{0}"), APP_TITLE).c_str(), e_menu_items::Wiki);
			submenu->addItem(lang_[L"Menus_ReportBugItem"].c_str(), e_menu_items::ReportBug);
			submenu->addSeparator();
			submenu->addItem(stringw(lang_[L"Menus_AboutItem"].c_str()).replace(stringw("{0}"), APP_TITLE).c_str(), e_menu_items::AboutApplication);

			elems.MenuBarMain->drop();

			video::ITexture* toolbarBorder = NULL;


			dimension2d<u32> screenSize = dev->getVideoDriver()->getScreenSize();

			elems.BottomBar = new CGUIDecentralisedToolBar(env, root, e_gui_elements::LoginToolbar, core::rect<s32>(0, 0, 10, 10));
			elems.BottomBar->setAlignment(EGUI_ALIGNMENT::EGUIA_UPPERLEFT, EGUI_ALIGNMENT::EGUIA_SCALE, EGUI_ALIGNMENT::EGUIA_LOWERRIGHT, EGUI_ALIGNMENT::EGUIA_LOWERRIGHT);
			elems.BottomBar->setRelativePosition(rect<s32>(0, screenSize.Height - 79, screenSize.Width, screenSize.Height));
			elems.BottomBar->drop();

			s32 y = 8;

			elems.LoginLabel = env->addStaticText(lang_[L"LoginBar_LogInLabel"].c_str(),
				rect< s32 >(10, y, 200, y + 18),
				false,
				true,
				elems.BottomBar,
				0);
			elems.LoginLabel->setOverrideColor(SColor(255, 255, 255, 255));
			elems.LoginLabel->setOverrideFont(boldFont);

			y += 22;

			elems.AvatarNameLabel = env->addStaticText(lang_[L"LoginBar_AvatarNameLabel"].c_str(),
				rect< s32 >(10, y, 200, y + 18),
				false,
				true,
				elems.BottomBar,
				0);
			elems.AvatarNameLabel->setOverrideColor(SColor(255, 255, 255, 255));

			elems.PasswordLabel = env->addStaticText(lang_[L"LoginBar_PasswordLabel"].c_str(),
				rect< s32 >(190, y, 390, y + 18),
				false,
				true,
				elems.BottomBar,
				0);
			elems.PasswordLabel->setOverrideColor(SColor(255, 255, 255, 255));

			y += 18;

			elems.AvatarCombo = new CGUIDecentralisedDropdown(env, elems.BottomBar, e_gui_elements::LoginToolbarAvatarCombo, rect<s32>(10, y, 185, y + 25));
			elems.AvatarCombo->setEnabled(false);
			((CGUIDecentralisedDropdown*)elems.AvatarCombo)->setText(lang_[L"LoginBar_NoAvatarsText"].c_str());
			elems.AvatarCombo->drop();

			elems.PasswordBox = new CGUIDecentralisedTextbox(L"", true, env, elems.BottomBar, e_gui_elements::LoginToolbarPasswordBox, rect<s32>(190, y, 318, y + 25));			
			elems.PasswordBox->drop();

			elems.PasswordBox->setEnabled(false);
			elems.PasswordBox->setPasswordBox(true, L'\x25cf');

			elems.LoginButton = new CGUIDecentralisedButton(env, elems.BottomBar, e_gui_elements::LoginToolbarLoginButton, rect<s32>(325, y, 380, y + 25));
			elems.LoginButton->setImages(elems.TxButtonLeft, elems.TxButtonMiddle, elems.TxButtonRight, elems.TxButtonPressedLeft, elems.TxButtonPressedMiddle, elems.TxButtonPressedRight);
			elems.LoginButton->setText(lang_[L"LoginBar_LoginButtonText"].c_str());
			elems.LoginButton->setEnabled(false);
			elems.LoginButton->drop();

			elems.CreateAvButton = new CGUIDecentralisedButton(env, elems.BottomBar, e_gui_elements::LoginToolbarCreateAvatarButton, rect<s32>(winSize.LowerRightCorner.X - 110, y, winSize.LowerRightCorner.X - 10, y + 25));
			elems.CreateAvButton->setImages(elems.TxButtonLeft, elems.TxButtonMiddle, elems.TxButtonRight, elems.TxButtonPressedLeft, elems.TxButtonPressedMiddle, elems.TxButtonPressedRight);
			elems.CreateAvButton->setText(lang_[L"LoginBar_CreateAvatarButtonText"].c_str());
			elems.CreateAvButton->setEnabled(true);
			elems.CreateAvButton->setIsPushButton(true);
			elems.CreateAvButton->setAutoExpand(false);
			elems.CreateAvButton->setAlignment(EGUI_ALIGNMENT::EGUIA_LOWERRIGHT, EGUI_ALIGNMENT::EGUIA_LOWERRIGHT,
				EGUI_ALIGNMENT::EGUIA_LOWERRIGHT, EGUI_ALIGNMENT::EGUIA_LOWERRIGHT);
			elems.CreateAvButton->drop();
		}

		void manager_gui::ToggleWindowCreateAvatar(std::wstring publicKey)
		{
			if (dialogs.CreateAvatar)
			{
				dialogs.CreateAvatar->RemoveDialog();
				delete dialogs.CreateAvatar;
				dialogs.CreateAvatar = NULL;
			}
			else
			{
			

				// add dialog
				dialogs.CreateAvatar = dialog_createavatar::AddDialog(elems, env, lang_, publicKey);
			}
		}

		void manager_gui::CleanupWindowCreateAvatar()
		{
			if (elems.CreateAvButton)
				elems.CreateAvButton->setPressed(false);

			if (dialogs.CreateAvatar)
			{
				delete dialogs.CreateAvatar;
				dialogs.CreateAvatar = NULL;
			}
		}

		void manager_gui::ShowWindowAbout()
		{
			if (dialogs.About)
				return;

			// add dialog
			dialogs.About = dialog_about::AddDialog(elems, env, lang_);
		}

		void manager_gui::CleanupWindowAbout()
		{
			if (dialogs.About)
			{
				delete dialogs.About;
				dialogs.About = NULL;
			}
		}

		void manager_gui::DrawAll()
		{
			env->drawAll();
		}

		stringw manager_gui::GetElementValue(s32 id)
		{
			IGUIElement* root = env->getRootGUIElement();
			IGUIElement* elem = root->getElementFromId(id, true);
			if (elem)
				return elem->getText();
			else
				return L"";
		}

		void manager_gui::ShowModal(s32 id)
		{
			rect<s32> rootRect = env->getRootGUIElement()->getAbsolutePosition();
			s32 centerX = rootRect.LowerRightCorner.X / 2;
			s32 centerY = rootRect.LowerRightCorner.Y / 2;

			std::wstring message = L"";
			if (id == e_modals::PasswordsDoNotMatch)
				message = lang_[L"CreateAvatar_ErrorPasswordMismatch"];
			else if (id == e_modals::AllFieldsMustBeCompleted)
				message = lang_[L"CreateAvatar_ErrorCompleteAll"];

			rect<s32> modalRect = rect<s32>(centerX - 180, centerY - 65, centerX + 180, centerY + 65);

			//IGUIGraphicWindow* modal = env->addVoodooWindow(modalRect,
			//	true,
			//	lang_[L"General_ErrorDialogTitle"].c_str(),
			//	0,
			//	e_gui_elements::ErrorModal,
			//	elems.TxWindowTitleLeft,
			//	elems.TxWindowTitleMiddle,
			//	elems.TxWindowTitleRight,
			//	elems.TxWindowLeft,
			//	elems.TxWindowRight,
			//	elems.TxWindowBottomLeft,
			//	elems.TxWindowBottomMiddle,
			//	elems.TxWindowBottomRight);
			//env->addStaticText(message.c_str(),
			//	rect<s32>(20, 45, 260, 89),
			//	false, true,
			//	modal);

			s32 centerModalX = (modalRect.LowerRightCorner.X - modalRect.UpperLeftCorner.X) / 2;

			//IGUIGraphicButton* modalButton = env->addVoodooButton(rect<s32>(centerModalX - 40, 85, centerModalX + 40, 110),
			//	modal,
			//	e_gui_elements::ErrorModalOkButton,
			//	lang_[L"General_DialogOkButton"].c_str(),
			//	elems.TxButtonLeft,
			//	elems.TxButtonMiddle,
			//	elems.TxButtonRight,
			//	elems.TxButtonPressedLeft,
			//	elems.TxButtonPressedMiddle,
			//	elems.TxButtonPressedRight);
			//modalButton->setAutoSize(false);
		}

		void manager_gui::AddConsoleLine(video::SColor textColor, std::wstring text)
		{
			if (elems.Console != NULL)
				elems.Console->addLine(textColor, text.c_str());
		}

		video::ITexture* manager_gui::loadTexture(IVideoDriver* driver, stringw skinPath, stringw filename)
		{
			stringw winBottomLeftPath = skinPath.c_str();
			winBottomLeftPath.append(filename);
			return driver->getTexture(winBottomLeftPath);
		}

		void manager_gui::SetCurrentTime(time_t currentTime)
		{
			if (elems.MenuBarMain != NULL)
			{
				//std::wstringstream ss;

				//wtime_facet* localized_facet = new wtime_facet(L"%H:%M%p");
				//static std::locale loc(std::wcout.getloc(), localized_facet);
				//ss.imbue(loc);
				//ss << currentTime;

				//elems.MenuBarMain->setTimeText(ss.str().c_str());
			}
		}
	}
}