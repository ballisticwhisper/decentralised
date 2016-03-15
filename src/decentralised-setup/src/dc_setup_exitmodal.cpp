#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#   include "wx/wx.h"
#endif

#include "dc_setup_exitmodal.h"

dc_setup_exitmodal::dc_setup_exitmodal(const wxString& title, wxSize& size, std::map<std::wstring, std::wstring>& lang)
	: wxDialog((wxDialog *)NULL, -1, title, wxDefaultPosition, wxSize(size)), m_lang(lang)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
}

dc_setup_exitmodal::~dc_setup_exitmodal()
{
}

wxBEGIN_EVENT_TABLE(dc_setup_exitmodal, wxDialog)
wxEND_EVENT_TABLE()