#ifndef __C_ABOUT_DIALOG_H_INCLUDED__
#define __C_ABOUT_DIALOG_H_INCLUDED__

#include <map>
#include "irrlicht.h"
#include "context_gui.h"
#include "IGUIDecentralisedDialog.h"
#include "CGUIDecentralisedDialog.h"

using namespace irr;
using namespace irr::core;
using namespace video;
using namespace gui;
using namespace decentralised;

namespace decentralised
{
	namespace dialogs
	{
		class dialog_about
		{
		public:
			~dialog_about();

			static dialog_about* AddDialog(context::context_gui &elems, IGUIEnvironment* env, std::map<std::wstring, std::wstring> &lang)
			{
				return new dialog_about(elems, env, lang);
			}

			void RemoveDialog();

			bool IsOpen() {
				if (window_)
					return window_->isVisible();

				return false;
			}

		private:
			dialog_about(context::context_gui &elems, IGUIEnvironment* env, std::map<std::wstring, std::wstring> &lang);

			void initElements();

			IGUIEnvironment* env_;
			IGUIDecentralisedDialog* window_;
			std::map<std::wstring, std::wstring> &lang_;
			context::context_gui &elems_;
		};
	}
}
#endif