#include "manager_core.h"

namespace decentralised
{
	namespace managers
	{
		manager_core::manager_core()
		{
			auto handle_event_received =
				std::bind(&manager_core::event_received, this, _1, _2);
			client = new decentralised_client(handle_event_received);
		}

		manager_core::~manager_core()
		{
			client->stop();
			delete client;
		}

		void manager_core::start()
		{
			client->start("data\\blockchain_data", false);
			avatars_ = client->loadAvatars("data\\avatars");

			uintptr_t myint = reinterpret_cast<uintptr_t>(&avatars_);

			SEvent e;
			e.EventType = EEVENT_TYPE::EET_USER_EVENT;
			e.UserEvent.UserData1 = 201;
			e.UserEvent.UserData2 = myint;
			receiver_->OnEvent(e);
		}

		void manager_core::stop()
		{
		}

		void manager_core::setEventReceiver(IEventReceiver* receiver)
		{
			receiver_ = receiver;
		}

		void manager_core::event_received(int code, std::string msg)
		{
			if (receiver_)
			{
				SEvent e;
				e.EventType = EEVENT_TYPE::EET_USER_EVENT;
				e.LogEvent.Text = msg.c_str();
				e.LogEvent.Level = ELOG_LEVEL::ELL_INFORMATION;
				receiver_->OnEvent(e);
			}

			printf(msg.append("\n").c_str());
		}

		void manager_core::addTransaction()
		{
			decentralised_transaction transaction;

			client->addDcTransaction(transaction);
		}

		void manager_core::createAvatar(std::string avatarsFolderPath, std::wstring firstName, std::wstring lastName, elliptic_curve_key keyPair)
		{
			client->createAvatar(avatarsFolderPath, firstName, lastName, keyPair);
		}
	}
}
