#ifndef __I_GUI_DECENTRALISED_WINDOW_H_INCLUDED__
#define __I_GUI_DECENTRALISED_WINDOW_H_INCLUDED__

#include "IGUIElement.h"
#include "EMessageBoxFlags.h"

namespace irr
{
namespace gui
{
	class IGUIButton;

	//! Default moveable window GUI element with border, caption and close icons.
	/** \par This element can create the following events of type EGUI_EVENT_TYPE:
	\li EGET_ELEMENT_CLOSED
	*/
	class IGUIDecentralisedWindow : public IGUIElement
	{
	public:

		//! constructor
		IGUIDecentralisedWindow(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle,
			irr::video::ITexture* titleLeft = NULL, irr::video::ITexture* titleMiddle = NULL, irr::video::ITexture* titleRight = NULL,
			irr::video::ITexture* winLeft = NULL, irr::video::ITexture* winRight = NULL,
			irr::video::ITexture* bottomLeft = NULL, irr::video::ITexture* bottomMiddle = NULL, irr::video::ITexture* bottomRight = NULL)
			: IGUIElement(EGUIET_WINDOW, environment, parent, id, rectangle) {}

		//! Returns pointer to the close button
		/** You can hide the button by calling setVisible(false) on the result. */
		virtual IGUIButton* getCloseButton() const = 0;

		//! Returns pointer to the minimize button
		/** You can hide the button by calling setVisible(false) on the result. */
		virtual IGUIButton* getMinimizeButton() const = 0;

		//! Returns pointer to the maximize button
		/** You can hide the button by calling setVisible(false) on the result. */
		virtual IGUIButton* getMaximizeButton() const = 0;

		//! Returns true if the window can be dragged with the mouse, false if not
		virtual bool isDraggable() const = 0;

		//! Sets whether the window can be dragged by the mouse
		virtual void setDraggable(bool draggable) = 0;

		//! Set if the window background will be drawn
		virtual void setDrawBackground(bool draw) = 0;

		//! Get if the window background will be drawn
		virtual bool getDrawBackground() const = 0;

		//! Set if the window titlebar will be drawn
		//! Note: If the background is not drawn, then the titlebar is automatically also not drawn
		virtual void setDrawTitlebar(bool draw) = 0;

		//! Get if the window titlebar will be drawn
		virtual bool getDrawTitlebar() const = 0;

		//! Returns the rectangle of the drawable area (without border and without titlebar)
		/** The coordinates are given relative to the top-left position of the gui element.<br>
		So to get absolute positions you have to add the resulting rectangle to getAbsolutePosition().UpperLeftCorner.<br>
		To get it relative to the parent element you have to add the resulting rectangle to getRelativePosition().UpperLeftCorner.
		Beware that adding a menu will not change the clientRect as menus are own gui elements, so in that case you might want to subtract
		the menu area additionally.	*/
		virtual core::rect<s32> getClientRect() const = 0;
	};


} // end namespace gui
} // end namespace irr

#endif

