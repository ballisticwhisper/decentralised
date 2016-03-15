#ifndef __DC_SETUP_FRAME_H_INCLUDED__
#define __DC_SETUP_FRAME_H_INCLUDED__

#include "../setup-background.xpm"
#include "wx/wxprec.h"
#include "lang.h"
#include "dc_setup_exitmodal.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

class dc_setup_frame : public wxFrame
{
public:

	dc_setup_frame(const wxString& title, int xpos, int ypos, int width, int height);

	~dc_setup_frame();

private:

	void OnLanguageChanged(wxCommandEvent& event);
	void OnCancelClicked(wxCommandEvent& event);

	wxStaticText *m_pTextCtrl;
	wxStaticBitmap *m_bitmapSidebar;
	wxChoice *m_langChoice;
	wxPanel *m_panel1;
	wxButton *m_nextButton;
	wxButton *m_cancelButton;
	wxIcon m_icon;

	std::map<std::wstring, std::wstring> m_lang;
	std::map<std::wstring, std::wstring> m_langNames;

	wxDECLARE_EVENT_TABLE();
};

enum
{
	BtnCancel = wxID_HIGHEST + 1
};

#endif