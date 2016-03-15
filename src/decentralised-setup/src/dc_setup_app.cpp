#include "wx/wxprec.h"
#include "dc_setup_app.h"
#include "dc_setup_frame.h"
#include "resource.h"

IMPLEMENT_APP(dc_setup_app)
 
// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool dc_setup_app::OnInit()
{
	dc_setup_frame *frame = new dc_setup_frame(
								_T("Decentralised Setup"),
								100, 100,
								520, 380);
	frame->CenterOnScreen();
	frame->SetBackgroundColour(wxColour(255, 255, 255, 255));	

	frame->Show(true);
	SetTopWindow(frame);
	return true;
}