//#ifndef __MANAGER_CORE_H_INCLUDED__
//#define __MANAGER_CORE_H_INCLUDED__
//
//#include <irrlicht.h>
//#include "dc-config.h"
//#include "decentralised_core.hpp"
//#include "core/avatar_file.hpp"
//#include <vector>
//#include <map>
//
//using namespace irr;
//
//using namespace irr::core;
//using namespace scene;
//using namespace video;
//using namespace io;
//using namespace gui;
//
//using namespace decentralised::core;
//
//namespace decentralised
//{
//	namespace managers
//	{
//		class manager_core
//		{
//		public:
//			manager_core();
//			~manager_core();
//
//			void start();
//			void stop();
//			void setEventReceiver(IEventReceiver* receiver);
//
//			void addTransaction();
//			void createAvatar(std::string avatarsFolderPath, std::wstring firstName, std::wstring lastName, elliptic_curve_key keyPair);
//
//		private:
//			void event_received(int code, std::string msg);
//
//			decentralised_client* client;
//			IEventReceiver* receiver_;
//			std::vector<avatar_file> avatars_;
//		};
//	}
//}
//
//#endif