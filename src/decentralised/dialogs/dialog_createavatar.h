#ifndef __C_CREATE_AVATAR_DIALOG_H_INCLUDED__
#define __C_CREATE_AVATAR_DIALOG_H_INCLUDED__

#include <map>
#include "irrlicht.h"
#include "context_gui.h"
#include "IGUIDecentralisedWindow.h"
#include "CGUIDecentralisedTextbox.h"

using namespace irr;
using namespace irr::core;
using namespace video;
using namespace gui;

namespace decentralised
{
	namespace dialogs
	{
		class dialog_createavatar
		{
		public:
			~dialog_createavatar();

			static dialog_createavatar* AddDialog(context::context_gui &elems, IGUIEnvironment* env, std::map<std::wstring, std::wstring> &lang, std::wstring publicKey)
			{
				return new dialog_createavatar(elems, env, lang, publicKey);
			}

			void RemoveDialog();

			bool IsOpen() {
				if (window_)
					return window_->isVisible();

				return false;
			}

		private:
			dialog_createavatar(context::context_gui &elems, IGUIEnvironment* env, std::map<std::wstring, std::wstring> &lang, std::wstring publicKey);

			void initElements(std::wstring publicKey);

			IGUIEnvironment* env_;
			IGUIDecentralisedWindow* window_;
			std::map<std::wstring, std::wstring> &lang_;
			context::context_gui &elems_;
		};
	}
}
#endif