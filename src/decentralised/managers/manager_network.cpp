#include "manager_network.h"

namespace decentralised
{
	namespace managers
	{
		manager_network::manager_network()
		{
			auto handle_event_received =
				std::bind(&manager_network::event_received, this, _1, _2);
			client = new decentralised_client(handle_event_received);
		}

		manager_network::~manager_network()
		{
			delete client;
		}

		void manager_network::initialize()
		{
			std::string message = client->get_genesis_message();
			printf(message.c_str());

			client->start("data\\blockchain_data");
		}

		void manager_network::event_received(int code, std::string msg)
		{
			printf(msg.append("\n").c_str());
		}
	}
}
