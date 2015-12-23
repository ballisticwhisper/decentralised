#ifndef INCLUDED_HELLOWORLDAPP_H
#define INCLUDED_HELLOWORLDAPP_H
 
// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

// The HelloWorldApp class. This class shows a window
// containing a statusbar with the text "Hello World"
class HelloWorldApp : public wxApp
{
public:
	virtual bool OnInit();
};
 
DECLARE_APP(HelloWorldApp)
 
#endif // INCLUDED_HELLOWORLDAPP_H