#include "wx/wxprec.h"
#include "dc_setup.h"
 
IMPLEMENT_APP(HelloWorldApp)
 
// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool HelloWorldApp::OnInit()
{
	wxFrame *frame = new wxFrame((wxFrame*) NULL, -1, _T("Hello wxWidgets World"));
	frame->CreateStatusBar();
	frame->SetStatusText(_T("Hello World"));
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}