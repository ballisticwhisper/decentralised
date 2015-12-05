// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_GUI_VOO_WINDOW_H_INCLUDED__
#define __C_GUI_VOO_WINDOW_H_INCLUDED__

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_GUI_
#include "IGUIEnvironment.h"

#include "IGUIDecentralisedWindow.h"
#include "CGUIDecentralisedButton.h"

namespace irr
{
namespace gui
{
	class IGUIButton;

	class CGUIDecentralisedWindow : public IGUIDecentralisedWindow
	{
	public:

		//! constructor
		CGUIDecentralisedWindow(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle,
			irr::video::ITexture* titleLeft, irr::video::ITexture* titleMiddle, irr::video::ITexture* titleRight,
			irr::video::ITexture* winLeft, irr::video::ITexture* winRight,
			irr::video::ITexture* bottomLeft, irr::video::ITexture* bottomMiddle, irr::video::ITexture* bottomRight);

		//! destructor
		virtual ~CGUIDecentralisedWindow();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! update absolute position
		virtual void updateAbsolutePosition();

		//! draws the element and its children
		virtual void draw();

		//! Returns pointer to the close button
		virtual IGUIButton* getCloseButton() const;

		//! Returns pointer to the minimize button
		virtual IGUIButton* getMinimizeButton() const;

		//! Returns pointer to the maximize button
		virtual IGUIButton* getMaximizeButton() const;

		//! Returns true if the window is draggable, false if not
		virtual bool isDraggable() const;

		//! Sets whether the window is draggable
		virtual void setDraggable(bool draggable);

        //! Set if the window background will be drawn
        virtual void setDrawBackground(bool draw);

        //! Get if the window background will be drawn
        virtual bool getDrawBackground() const;

        //! Set if the window titlebar will be drawn
        //! Note: If the background is not drawn, then the titlebar is automatically also not drawn
        virtual void setDrawTitlebar(bool draw);

        //! Get if the window titlebar will be drawn
        virtual bool getDrawTitlebar() const;

		//! Returns the rectangle of the drawable area (without border and without titlebar)
		virtual core::rect<s32> getClientRect() const;

		//! Writes attributes of the element.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const;

		//! Reads attributes of the element
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

	protected:

		void updateClientRect();
		void refreshSprites();

		IGUIButton* CloseButton;
		IGUIButton* MinButton;
		IGUIButton* RestoreButton;
		core::rect<s32> ClientRect;
		video::SColor CurrentIconColor;

		core::position2d<s32> DragStart;
		bool Dragging, IsDraggable;
        bool DrawBackground;
        bool DrawTitlebar;
		bool IsActive;

		video::ITexture* TitleLeft;
		video::ITexture* TitleMiddle;
		video::ITexture* TitleRight;

		video::ITexture* WinLeft;
		video::ITexture* WinRight;

		video::ITexture* BottomLeft;
		video::ITexture* BottomMiddle;
		video::ITexture* BottomRight;
	};

} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

#endif

