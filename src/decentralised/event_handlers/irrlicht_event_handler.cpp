#include "irrlicht_event_handler.h"

bool irrlicht_event_handler::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_GUI_EVENT)
	{
		if (event.GUIEvent.Caller->getID() == e_gui_elements::LoginToolbarCreateAvatarButton
			&& event.GUIEvent.EventType == EGUI_EVENT_TYPE::EGET_BUTTON_CLICKED)
		{
			this->Context.gui_manager->ToggleWindowCreateAvatar();
		}
		else if (event.GUIEvent.Caller->getID() == e_gui_elements::CreateAvCreateButton
			&& event.GUIEvent.EventType == EGUI_EVENT_TYPE::EGET_BUTTON_CLICKED)
		{
			stringw firstName = this->Context.gui_manager->GetElementValue(e_gui_elements::CreateAvFirstnameBox);
			stringw lastName = this->Context.gui_manager->GetElementValue(e_gui_elements::CreateAvLastnameBox);
			stringw password = this->Context.gui_manager->GetElementValue(e_gui_elements::CreateAvPasswordBox);
			stringw confirm = this->Context.gui_manager->GetElementValue(e_gui_elements::CreateAvConfirmBox);

			//EVooModals result = this->Context.AccountManager->CreateAccount(firstName,
			//	lastName,
			//	password,
			//	confirm);
			//this->Context.gui_manager->ShowModal(result);
		}
		else if (event.GUIEvent.Caller->getID() == e_gui_elements::WindowCreateAvatar
			&& event.GUIEvent.EventType == EGUI_EVENT_TYPE::EGET_ELEMENT_CLOSED)
		{
			this->Context.gui_manager->CleanupWindowCreateAvatar();
		}
		else if (event.GUIEvent.Caller->getID() == e_gui_elements::WindowAbout
			&& event.GUIEvent.EventType == EGUI_EVENT_TYPE::EGET_ELEMENT_CLOSED)
		{
			this->Context.gui_manager->CleanupWindowAbout();
		}
		else if (event.GUIEvent.Caller->getID() == e_gui_elements::AboutLink
			&& event.GUIEvent.EventType == EGUI_EVENT_TYPE::EGET_BUTTON_CLICKED)
		{
#ifdef _IRR_WINDOWS_
			ShellExecute(0, 0, "http://decentralised-project.org", 0, 0, SW_SHOW);
#endif
// TODO: Linux implementation
		}
		else if (event.GUIEvent.EventType == EGUI_EVENT_TYPE::EGET_MENU_ITEM_SELECTED)
		{
			gui::IGUIContextMenu* item = (gui::IGUIContextMenu*)event.GUIEvent.Caller;
			s32 id = item->getItemCommandId(item->getSelectedItem());
			switch (id)
			{
			case e_menu_items::ExitApplication:
				this->Context.device->closeDevice();
				break;
			case e_menu_items::Wiki:
#ifdef _IRR_WINDOWS_
				ShellExecute(0, 0, "http://decentralised-project.org/wiki", 0, 0, SW_SHOW);
#endif
				break;
			case e_menu_items::ReportBug:
#ifdef _IRR_WINDOWS_
				ShellExecute(0, 0, "http://decentralised-project.org/bug-report", 0, 0, SW_SHOW);
#endif
				break;
			case e_menu_items::AboutApplication:
				this->Context.gui_manager->ShowWindowAbout();
				break;
			default:
				this->Context.gui_manager->AddConsoleLine(video::SColor(255, 255, 255, 255), std::wstring(item->getItemText(item->getSelectedItem())));
				break;
			}
		}
		return false;
	}
	return false;
}