#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#   include "wx/wx.h"
#endif

#include "dc_setup_frame.h"

dc_setup_frame::dc_setup_frame(const wxString& title, int xpos, int ypos, int width, int height)
	: wxFrame((wxFrame *)NULL, -1, title, wxPoint(xpos, ypos), wxSize(width, height))
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	// Create split layout
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer(wxHORIZONTAL);

	// Set up left bitmap from an embedded file
	wxImage::AddHandler(new wxJPEGHandler());

	wxBoxSizer* bSizerLeft;
	bSizerLeft = new wxBoxSizer(wxVERTICAL);

	wxBitmap b(setup_background);
	m_bitmapSidebar = new wxStaticBitmap(this, wxID_ANY, (wxGDIImage&)b, wxDefaultPosition, wxDefaultSize, 0);
	bSizerLeft->Add(m_bitmapSidebar, 0, wxALL, 0);

	bSizerMain->Add(bSizerLeft, 3, wxEXPAND, 0);

	// Create right panel
	wxBoxSizer* bSizerRight;
	bSizerRight = new wxBoxSizer(wxVERTICAL);

	m_panel1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* bSizerPanel;
	bSizerPanel = new wxBoxSizer(wxVERTICAL);

	m_pTextCtrl = new wxStaticText(m_panel1, -1, wxT("Select Language"),
		wxPoint(20, 20), wxDefaultSize);
	bSizerPanel->Add(m_pTextCtrl, 0, wxALL, 5);

	wxArrayString m_langChoices;
	m_langChoices.Add(wxT("English"));
	m_langChoices.Add(wxT("Português"));
	m_langChoices.Add(wxT("Pусский"));
	m_langChoice = new wxChoice(m_panel1, wxID_ANY, wxDefaultPosition, wxSize(180, 23), m_langChoices, 0);
	m_langChoice->Select(0);

	bSizerPanel->Add(m_langChoice, 0, wxALL, 5);

	m_panel1->SetSizer(bSizerPanel);
	//bSizerPanel->Fit(m_panel1);



	//m_pTextCtrl->Wrap(180);

	//wxFont f(14, wxFONTFAMILY_DEFAULT,
	//	wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Verdana");
	//m_pTextCtrl->SetFont(f);

	bSizerRight->Add(m_panel1, 1, wxALL | wxEXPAND, 100);

	wxBoxSizer* bSizerFooter;
	bSizerFooter = new wxBoxSizer(wxHORIZONTAL);

	m_cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	bSizerFooter->Add(m_cancelButton, 0, wxALL, 5);

	m_nextButton = new wxButton(this, wxID_ANY, wxT("Next"), wxDefaultPosition, wxDefaultSize, 0);
	bSizerFooter->Add(m_nextButton, 0, wxALL, 5);

	bSizerRight->Add(bSizerFooter, 0, wxALL | wxALIGN_RIGHT, 15);


	bSizerMain->Add(bSizerRight, 7, wxEXPAND, 5);

	m_panel1->Layout();

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