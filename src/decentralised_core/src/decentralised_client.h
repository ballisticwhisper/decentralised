#ifndef __DECENTRALISED_CLIENT_H_INCLUDED__
#define __DECENTRALISED_CLIENT_H_INCLUDED__

#include "dc-config.h"
#include <bitcoin/bitcoin.hpp>
#include <vector>
#include <map>

using namespace libbitcoin;

namespace decentralised
{
	namespace core
	{
		class decentralised_client
		{
		public:
			decentralised_client();
			~decentralised_client();

			std::string decentralised_client::get_genesis_message();

		private:
			threadpool disk_pool;
			threadpool net_pool;
		};
	}
}

#endif