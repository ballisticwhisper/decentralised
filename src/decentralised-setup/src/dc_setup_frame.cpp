#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#   include "wx/wx.h"
#endif

#include "dc_setup_frame.h"

dc_setup_frame::dc_setup_frame(const wxString& title, int xpos, int ypos, int width, int height)
	: wxFrame((wxFrame *)NULL, -1, title, wxPoint(xpos, ypos), wxSize(width, height))
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	std::string sysLang = "en";

#ifdef WIN32
	WCHAR wcBuffer[512];
	int res = GetSystemDefaultLocaleName(wcBuffer, 512);
	std::wstring sysLangW(wcBuffer);
	sysLangW = sysLangW.substr(0, 2);
	sysLang = std::string(sysLangW.begin(), sysLangW.end());
	if (sysLang.size() < 2)
		sysLang = "en";
#endif
	
	// Load the default system language, or english if it's not available.
	lang langReader;
	m_lang = langReader.loadLanguage(sysLang);

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

	m_pTextCtrl = new wxStaticText(m_panel1, -1, m_lang[L"Step1_SelectLang"].c_str(),
		wxPoint(20, 20), wxDefaultSize);
	bSizerPanel->Add(m_pTextCtrl, 0, wxALL, 5);

	m_langNames.clear();
	m_langNames = langReader.loadLanguageNames();
	wxArrayString m_langChoices;

	int selectedIndex = 0;
	int currentIndex = 0;
	for (std::map<std::wstring, std::wstring>::iterator it = m_langNames.begin(); it != m_langNames.end(); ++it)
	{
		if (it->first == sysLang)
			selectedIndex = currentIndex;

		m_langChoices.Add(it->second);
		currentIndex++;
	}

	m_langChoice = new wxChoice(m_panel1, wxID_ANY, wxDefaultPosition, wxSize(180, 23), m_langChoices, 0);
	m_langChoice->Select(selectedIndex);

	bSizerPanel->Add(m_langChoice, 0, wxALL, 5);

	m_panel1->SetSizer(bSizerPanel);
	//bSizerPanel->Fit(m_panel1);



	//m_pTextCtrl->Wrap(180);

	//wxFont f(14, wxFONTFAMILY_DEFAULT,
	//	wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL;, false, "Verdana");
	//m_pTextCtrl->SetFont(f);

	bSizerRight->Add(m_panel1, 1, wxALL | wxEXPAND, 100);

	wxBoxSizer* bSizerFooter;
	bSizerFooter = new wxBoxSizer(wxHORIZONTAL);

	m_cancelButton = new wxButton(this, BtnCancel, m_lang[L"General_Cancel"].c_str(), wxDefaultPosition, wxDefaultSize, 0);
	bSizerFooter->Add(m_cancelButton, 0, wxALL, 5);

	m_nextButton = new wxButton(this, wxID_ANY, m_lang[L"General_Next"].c_str(), wxDefaultPosition, wxDefaultSize, 0);
	bSizerFooter->Add(m_nextButton, 0, wxALL, 5);

	bSizerRight->Add(bSizerFooter, 0, wxALL | wxALIGN_RIGHT, 15);


	bSizerMain->Add(bSizerRight, 7, wxEXPAND, 5);

	m_panel1->Layout();

	m_icon = wxICON(_(IDI_APP_ICON));
	this->SetIcon(m_icon);

	this->SetSizer(bSizerMain);
	this->Layout();
	this->Centre(wxBOTH);
	this->SetWindowStyle(wxMINIMIZE_BOX | wxCLOSE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);

	Layout();
}

dc_setup_frame::~dc_setup_frame()
{
}

void dc_setup_frame::OnLanguageChanged(wxCommandEvent& event)
{
	std::wstring langName = event.GetString();
	std::string langCode;

	for (std::map<std::wstring, std::wstring>::iterator it = m_langNames.begin(); it != m_langNames.end(); ++it)
	{
		if (it->second == langName)
		{
			langCode = std::string(it->first.begin(), it->first.end());
			break;
		}
	}

	lang langReader;
	m_lang = langReader.loadLanguage(langCode);

	m_pTextCtrl->SetLabelText(m_lang[L"Step1_SelectLang"].c_str());
	m_cancelButton->SetLabelText(m_lang[L"General_Cancel"].c_str());
	m_nextButton->SetLabelText(m_lang[L"General_Next"].c_str());

	this->SetFocus();
}

void dc_setup_frame::OnCancelClicked(wxCommandEvent& event)
{
	dc_setup_exitmodal dlg(m_lang[L"Exit_Title"].c_str(), wxSize(300, 200), m_lang);
	dlg.CenterOnParent();

	if (dlg.ShowModal())
	{
		//Close(TRUE);
	}

}

wxBEGIN_EVENT_TABLE(dc_setup_frame, wxFrame)
EVT_CHOICE(wxID_ANY, dc_setup_frame::OnLanguageChanged)
EVT_BUTTON(BtnCancel, dc_setup_frame::OnCancelClicked)
wxEND_EVENT_TABLE()

