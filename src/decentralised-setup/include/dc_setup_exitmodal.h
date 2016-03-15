#ifndef __DC_SETUP_EXITMODAL_H_INCLUDED__
#define __DC_SETUP_EXITMODAL_H_INCLUDED__

#include "wx/wxprec.h"
#include "lang.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

class dc_setup_exitmodal : public wxDialog
{
public:

	dc_setup_exitmodal(const wxString& title, wxSize& size, std::map<std::wstring, std::wstring>& lang);

	~dc_setup_exitmodal();

private:

	wxStaticText *m_pTextCtrl;
	wxButton *m_exitButton;
	wxButton *m_cancelButton;

	std::map<std::wstring, std::wstring> m_lang;

	wxDECLARE_EVENT_TABLE();
};

enum
{
	BtnExit = wxID_HIGHEST + 1	
};

#endif