#ifndef __DC_SETUP_APP_H_INCLUDED__
#define __DC_SETUP_APP_H_INCLUDED__
 
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

class dc_setup_app : public wxApp
{
public:
	virtual bool OnInit();
};
 
DECLARE_APP(dc_setup_app)
 
#endif