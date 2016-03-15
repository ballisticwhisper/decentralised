#ifndef __C_CREATE_AVATAR_DIALOG_H_INCLUDED__
#define __C_CREATE_AVATAR_DIALOG_H_INCLUDED__

#include <map>
#include "irrlicht.h"
#include "context_gui.h"
#include "IGUIDecentralisedDialog.h"
#include "CGUIDecentralisedDialog.h"
#include "CGUIDecentralisedTextbox.h"
#include <decentralised_core.hpp>

using namespace decentralised::context;
using namespace decentralised::core;
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

			static dialog_createavatar* AddDialog(context::context_gui &elems, IGUIEnvironment* env, std::map<std::wstring, std::wstring> &lang, elliptic_curve_key &keyPair)
			{
				dialog_createavatar* d = new dialog_createavatar(elems, env, lang, keyPair);
				d->InitElements();
				return d;
			}

			void RemoveDialog();
			void InitElements();

			bool IsOpen() {
				if (window_)
					return window_->isVisible();

				return false;
			}

		private:
			dialog_createavatar(context::context_gui &elems, IGUIEnvironment* env, std::map<std::wstring, std::wstring> &lang, elliptic_curve_key &keyPair);

			IGUIEnvironment* env_;
			IGUIDecentralisedDialog* window_;
			std::map<std::wstring, std::wstring> &lang_;
			context::context_gui &elems_;
			elliptic_curve_key &keyPair_;
		};
	}
}
#endif