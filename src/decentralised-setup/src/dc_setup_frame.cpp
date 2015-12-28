#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#   include "wx/wx.h"
#endif

#include "dc_setup_frame.h"

dc_setup_frame::dc_setup_frame(const wxString& title, int xpos, int ypos, int width, int height)
	: wxFrame((wxFrame *)NULL, -1, title, wxPoint(xpos, ypos), wxSize(width, height))
{
	wxImage::AddHandler(new wxJPEGHandler());

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizerLeft;
	bSizerLeft = new wxBoxSizer(wxVERTICAL);

	wxBitmap b(setup_background);

	m_bitmapSidebar = new wxStaticBitmap(this, wxID_ANY, (wxGDIImage&)b, wxDefaultPosition, wxDefaultSize, 0);
	bSizerLeft->Add(m_bitmapSidebar, 0, wxALL, 0);


	bSizerMain->Add(bSizerLeft, 3, wxEXPAND, 5);

	wxBoxSizer* bSizerRight;
	bSizerRight = new wxBoxSizer(wxVERTICAL);

	m_pTextCtrl = new wxStaticText(this, -1, wxT("Welcome to the Decentralised setup wizard"),
		wxPoint(20, 20), wxDefaultSize);
	m_pTextCtrl->Wrap(180);

	wxFont f(14, wxFONTFAMILY_DEFAULT,
		wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Verdana");
	m_pTextCtrl->SetFont(f);
	bSizerRight->Add(m_pTextCtrl, 0, wxALL, 10);


	bSizerMain->Add(bSizerRight, 7, wxEXPAND, 5);


	this->SetSizer(bSizerMain);
	this->Layout();

	this->Centre(wxBOTH);




	this->SetWindowStyle(wxCAPTION | wxMINIMIZE_BOX | wxCLOSE_BOX);

	// More will be added here later

	Layout();
}

dc_setup_frame::~dc_setup_frame()
{
}