#ifndef __DECENTRALISED_CLIENT_H_INCLUDED__
#define __DECENTRALISED_CLIENT_H_INCLUDED__

#include "dc-config.h"
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/blockchain.hpp>
#include <bitcoin/node.hpp>
#include <leveldb/db.h>
#include <vector>
#include <map>

using namespace bc;
using namespace bc::chain;
using namespace bc::network;
using namespace bc::node;

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

			void start(const char prefix[]);

		private:
			leveldb::Options create_open_options();

			threadpool disk_pool;
			threadpool net_pool;
		};
	}
}

#endif