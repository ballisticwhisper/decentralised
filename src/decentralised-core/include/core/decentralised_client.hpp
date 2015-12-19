#ifndef __DECENTRALISED_CLIENT_H_INCLUDED__
#define __DECENTRALISED_CLIENT_H_INCLUDED__

#include "dc-config.h"
#include <leveldb/db.h>
#include <core/block.hpp>
#include <core/utility/mmfile.hpp>
#include <core/utility/serializer.hpp>
#include <core/threadpool.hpp>
#include <core/blockchain/leveldb_blockchain.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <future>

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
			void create_stealth_db(const std::string &filename);
			void create_file(const std::string& filename, size_t filesize);
			threadpool* pool;
			leveldb_blockchain* chain;
		};
	}
}

#endif