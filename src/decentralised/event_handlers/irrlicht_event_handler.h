#ifndef __IRRLICHT_EVENT_HANDLER_H_INCLUDED__
#define __IRRLICHT_EVENT_HANDLER_H_INCLUDED__

#include <irrlicht.h>
#include "context_application.h"
#include "context_gui.h"
#include <decentralised_core.hpp>

#ifdef _IRR_WINDOWS_
#include <windows.h>
#include <shellapi.h>
#pragma comment(lib,"shell32.lib")
#endif

using namespace irr;
using namespace decentralised::context;
using namespace decentralised::core;

class irrlicht_event_handler : public IEventReceiver
{
public:
	irrlicht_event_handler(context_application& context) : Context(context) { }

	virtual bool OnEvent(const SEvent& event);

private:
	context_application& Context;
};

#endif