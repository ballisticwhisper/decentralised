#ifndef __DC_SETUP_FRAME_H_INCLUDED__
#define __DC_SETUP_FRAME_H_INCLUDED__

#include "../setup-background.xpm"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

class dc_setup_frame : public wxFrame
{
public:

	dc_setup_frame(const wxString& title, int xpos, int ypos, int width, int height);

	~dc_setup_frame();

private:

	wxStaticText *m_pTextCtrl;
	wxStaticBitmap *m_bitmapSidebar;
};

#endif