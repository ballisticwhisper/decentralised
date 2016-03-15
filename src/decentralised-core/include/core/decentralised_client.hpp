#ifndef __DECENTRALISED_CLIENT_H_INCLUDED__
#define __DECENTRALISED_CLIENT_H_INCLUDED__

#include "dc-config.h"
#include <leveldb/db.h>
#include <core/block.hpp>
#include <core/utility/mmfile.hpp>
#include <core/utility/serializer.hpp>
#include <core/utility/elliptic_curve_key.hpp>
#include <core/threadpool.hpp>
#include <core/poller.hpp>
#include <core/session.hpp>
#include <core/transaction_pool.hpp>
#include <core/avatar_file.hpp>
#include <core/blockchain/leveldb_blockchain.hpp>
#include <core/network/network.hpp>
#include <core/network/hosts.hpp>
#include <core/network/handshake.hpp>
#include <core/network/protocol.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <future>

namespace decentralised
{
	namespace core
	{
		enum decentralised_transaction_type
		{
			RegisterAvatar = 1
		};

		struct decentralised_transaction
		{
			decentralised_transaction_type type;
			std::string param1;
			std::string param2;
			std::string param3;
			int paramInt;
			float paramFloat;
		};

		using std::placeholders::_1;
		using std::placeholders::_2;
		using std::placeholders::_3;

		class decentralised_client
		{
		public:
			typedef std::function<void(const int, const std::string)> signal_handler;

			decentralised_client(signal_handler eventHandler);
			~decentralised_client();

			std::string decentralised_client::get_genesis_message();

			void start(const char prefix[], bool isSeedOnly);
			void stop();

			void addDcTransaction(decentralised_transaction transaction);

			void createAvatar(std::string avatarsFolderPath, std::wstring firstName, std::wstring lastName, elliptic_curve_key keyPair);
			std::vector<avatar_file> loadAvatars(std::string avatarsFolderPath);

		private:
			//void blockchain_started(const std::error_code& ec);
			void height_fetched(const std::error_code& ec, size_t last_height);
			void imported_genesis(const std::error_code& ec);
			void display_block_header(const std::error_code& ec, const block_header_type& header);
			void session_started(const std::error_code& ec);
			void connection_started(const std::error_code& ec, channel_ptr node);
			void recv_tx(const std::error_code& ec, const transaction_type& tx, channel_ptr node);
			void new_unconfirm_valid_tx(const std::error_code& ec, const index_list& unconfirmed, const transaction_type& tx);

			threadpool disk_pool;
			threadpool net_pool;
			threadpool mem_pool;

			leveldb_blockchain chain;

			std::string dbPath;
			p2p_network p2p;
			transaction_pool txpool;
			hosts hosts;
			handshake handshake;
			network network;
			protocol protocol;
			poller poller;
			session session;
			signal_handler eventHandler_;
			bool isConnectionFound;
		};
	}
}

#endif