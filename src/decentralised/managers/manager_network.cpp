#include "manager_network.h"

namespace decentralised
{
	namespace managers
	{
		manager_network::manager_network()
		{
			client = new decentralised_client();
		}

		manager_network::~manager_network()
		{
			delete client;
		}

		void manager_network::initialize()
		{
			std::string message = client->get_genesis_message();
			printf(message.c_str());
		}
	}
}
