#ifndef __DECENTRALISED_CLIENT_H_INCLUDED__
#define __DECENTRALISED_CLIENT_H_INCLUDED__

#include "dc-config.h"
#include <leveldb/db.h>
#include <core/block.hpp>
#include <core/utility/mmfile.hpp>
#include <core/utility/serializer.hpp>
#include <core/threadpool.hpp>
#include <core/blockchain/leveldb_blockchain.hpp>
#include <core/network/network.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <future>

namespace decentralised
{
	namespace core
	{
		using std::placeholders::_1;
		using std::placeholders::_2;

		class decentralised_client
		{
		public:
			typedef std::function<void(const int, const std::string)> signal_handler;

			decentralised_client(signal_handler eventHandler);
			~decentralised_client();

			std::string decentralised_client::get_genesis_message();

			void start(const char prefix[]);

		private:
			void blockchain_started(const std::error_code& ec);
			void height_fetched(const std::error_code& ec, size_t last_height);
			void imported_genesis(const std::error_code& ec);
			void display_block_header(const std::error_code& ec, const block_header_type& header);

			threadpool disk_pool;
			threadpool net_pool;
			threadpool mem_pool;
			leveldb_blockchain chain;
			network net;
			signal_handler eventHandler_;
		};
	}
}

#endif