#ifndef __C_GUI_DC_DIALOG_H_INCLUDED__
#define __C_GUI_DC_DIALOG_H_INCLUDED__

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_GUI_
#include "IGUIEnvironment.h"

#include "IGUIDecentralisedDialog.h"
#include "CGUIDecentralisedButton.h"

namespace irr
{
namespace gui
{
	class IGUIButton;

	class CGUIDecentralisedDialog : public IGUIDecentralisedDialog
	{
	public:

		//! constructor
		CGUIDecentralisedDialog(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle, bool hasTitleBar = true);

		//! destructor
		virtual ~CGUIDecentralisedDialog();

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

		virtual void setDialogSkin(irr::video::ITexture *background, irr::video::ITexture *foreground);

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

		video::ITexture* background_;
		video::ITexture* foreground_;
	};

} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

#endif

