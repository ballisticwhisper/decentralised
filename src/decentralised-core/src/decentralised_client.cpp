#include "core/decentralised_client.hpp"
#include <boost/filesystem.hpp>

using boost::filesystem::path;

namespace decentralised
{
	namespace core
	{
		decentralised_client::decentralised_client(signal_handler eventHandler) :
			disk_pool(4),
			net_pool(1),
			mem_pool(1),
			p2p(p2p_network_type::decentralised_main),
			chain(disk_pool),
			hosts(net_pool), handshake(net_pool, p2p), network(net_pool),
			protocol(net_pool, hosts, handshake, network, p2p),
			poller(mem_pool, chain),
			txpool(mem_pool, chain),
			session(net_pool, { handshake, protocol, chain, poller, txpool }),
			eventHandler_(eventHandler)
		{
			isConnectionFound = false;
		}

		decentralised_client::~decentralised_client()
		{
			// Stop threadpools.
			net_pool.stop();
			disk_pool.stop();
			mem_pool.stop();

			// Join threadpools. Wait for them to finish.
			net_pool.join();
			disk_pool.join();
			mem_pool.join();

			// Safely close blockchain database.
			chain.stop();
		}

		std::string decentralised_client::get_genesis_message()
		{
			block_type blk = genesis_block();
			const transaction_type& coinbase_tx = blk.transactions[0];
			const transaction_input_type& coinbase_input = coinbase_tx.inputs[0];
			const script_type& input_script = coinbase_input.script;
			const data_chunk& raw_block_message = save_script(input_script);

			return std::string(raw_block_message.begin() + 8, raw_block_message.end()).append("\n");
		}

		void decentralised_client::addDcTransaction(decentralised_transaction transaction)
		{
			transaction_type transactionType;
			transactionType.version = 1;
			transactionType.locktime = 0;

			// TODO: Need an input before we can make a transaction, which means we need to mine a block.

			//transaction_input_type input;

			//transactionType.inputs.insert();

			//txpool.store(transactionType, confirmHandler, validateHandler);
		}

		std::vector<avatar_file> decentralised_client::loadAvatars(std::string avatarsFolderPath)
		{
			avatar_file avatarFile;
			return avatarFile.loadAll(avatarsFolderPath);
		}

		void decentralised_client::createAvatar(std::string avatarsFolderPath, std::wstring firstName, std::wstring lastName, elliptic_curve_key keyPair)
		{
			std::string filename = std::string(std::string(firstName.begin(), firstName.end()))
				.append("-")
				.append(std::string(lastName.begin(), lastName.end()))
				.append(".dat");

			path db_path = path(avatarsFolderPath) / filename.c_str();

			avatar_file avatar;
			avatar.create(db_path.generic_string().c_str(), firstName, lastName, keyPair);
		}

		void decentralised_client::stop()
		{
			std::promise<std::error_code> ec_promise;
			auto session_stopped =
				[&ec_promise](const std::error_code& ec)
			{
				ec_promise.set_value(ec);
			};
			session.stop(session_stopped);
			std::error_code ec = ec_promise.get_future().get();
			if (ec)
				log_error() << "Problem stopping session: " << ec.message();
		}

		void decentralised_client::start(const char prefix[], bool isSeedOnly)
		{
			protocol.subscribe_channel(
				std::bind(&decentralised_client::connection_started, this, _1, _2));

			std::string dbPath = std::string(prefix);

			std::promise<std::error_code> ec_chain;
			auto blockchain_started = [&](const std::error_code& ec)
			{
				if (ec)
				{
					eventHandler_(-500, std::string("Error: ").append(ec.message()));
					return;
				}
				eventHandler_(1, "Blockchain started.");

				auto handle_height_fetched =
					std::bind(&decentralised_client::height_fetched, this, _1, _2);

				chain.fetch_last_height(handle_height_fetched);

				ec_chain.set_value(ec);
			};

			chain.start(dbPath, blockchain_started);

			std::error_code ec = ec_chain.get_future().get();
			if (ec)
			{
				eventHandler_(1, std::string("Problem starting blockchain: ").append(ec.message()).c_str());
				return;
			}

			txpool.start();

			auto handle_start = std::bind(&decentralised_client::session_started, this, _1);
			session.start(handle_start, isSeedOnly);
		}

		void decentralised_client::connection_started(const std::error_code& ec, channel_ptr node)
		{
			if (ec)
			{
				log_warning() << "Couldn't start connection: " << ec.message();
				return;
			}

			//if (!isConnectionFound)
			//{
			//	isConnectionFound = true;
			//	eventHandler_(1, "Connected to network.");
			//}

			// Subscribe to transaction messages from this node.
			node->subscribe_transaction(
				std::bind(&decentralised_client::recv_tx, this, _1, _2, node));
			// Stay subscribed to new connections.
			protocol.subscribe_channel(
				std::bind(&decentralised_client::connection_started, this, _1, _2));
		}

		void decentralised_client::session_started(const std::error_code& ec)
		{
			if (ec)
				log_error() << "dc-client: " << ec.message();
		}

		void decentralised_client::recv_tx(const std::error_code& ec, const transaction_type& tx, channel_ptr node)
		{
			if (ec)
			{
				log_error() << "Receive transaction: " << ec.message();
				return;
			}

			// Called when the transaction becomes confirmed in a block.
			auto handle_confirm = [](const std::error_code& ec)
			{
				if (ec)
					log_error() << "Confirm error: " << ec.message();
			};
			// Validate the transaction from the network.
			// Attempt to store in the transaction pool and check the result.
			txpool.store(tx, handle_confirm,
				std::bind(&decentralised_client::new_unconfirm_valid_tx, this, _1, _2, tx));
			// Resubscribe to transaction messages from this node.
			node->subscribe_transaction(
				std::bind(&decentralised_client::recv_tx, this, _1, _2, node));
		}

		void decentralised_client::new_unconfirm_valid_tx(const std::error_code& ec, const index_list& unconfirmed, const transaction_type& tx)
		{
			const hash_digest& tx_hash = hash_transaction(tx);
			if (ec)
			{
				log_error()
					<< "Error storing memory pool transaction "
					<< tx_hash << ": " << ec.message();
			}
			else
			{
				auto l = log_info();
				l << "Accepted transaction ";
				if (!unconfirmed.empty())
				{
					l << "(Unconfirmed inputs";
					for (auto idx : unconfirmed)
						l << " " << idx;
					l << ") ";
				}
				l << tx_hash;
			}
		}

		//void decentralised_client::blockchain_started(const std::error_code& ec)
		//{
		//	if (ec)
		//	{
		//		eventHandler_(-500, std::string("Error: ").append(ec.message()));
		//		return;
		//	}
		//	eventHandler_(1, "Blockchain started.");

		//	auto handle_height_fetched =
		//		std::bind(&decentralised_client::height_fetched, this, _1, _2);

		//	chain.fetch_last_height(handle_height_fetched);
		//}

		void decentralised_client::height_fetched(const std::error_code& ec, size_t last_height)
		{
			if (ec)
			{
				if (ec == error::not_found)
				{
					eventHandler_(2, "No existing blocks in the blockchain database.");

					block_type first_block = genesis_block();

					auto handle_imported_genesis =
						std::bind(&decentralised_client::imported_genesis, this, _1);

					chain.import(first_block, 0, handle_imported_genesis);
				}
				else
				{
					eventHandler_(-501, std::string("Error fetching blockhain height: ").append(ec.message()));
				}
				return;
			}

			eventHandler_(3, std::string("Blockchain database height: ").append(std::to_string(last_height)));

			auto handle_display_block_header =
				std::bind(&decentralised_client::display_block_header, this, _1, _2);

			chain.fetch_block_header(last_height, handle_display_block_header);
		}

		void decentralised_client::imported_genesis(const std::error_code& ec)
		{
			if (ec)
			{
				eventHandler_(-502, std::string("Importing genesis block failed: ").append(ec.message()));
				return;
			}
			eventHandler_(4, "Imported genesis block.");

			auto handle_height_fetched =
				std::bind(&decentralised_client::height_fetched, this, _1, _2);

			chain.fetch_last_height(handle_height_fetched);
		}

		void decentralised_client::display_block_header(const std::error_code& ec, const block_header_type& header)
		{
			if (ec)
			{
				eventHandler_(-503, std::string("Loading last block header failed: ").append(ec.message()));
				return;
			}

			const hash_digest& blk_hash = hash_block_header(header);
			printf(std::string("Hash: ").append(encode_hex(blk_hash)).append("\n").c_str());
			printf(std::string("Version: ").append(std::to_string(header.version)).append("\n").c_str());
			printf(std::string("Prev Block Hash: ").append(encode_hex(header.previous_block_hash)).append("\n").c_str());
			printf(std::string("Merkle: ").append(encode_hex(header.merkle)).append("\n").c_str());
			printf(std::string("Timestamp: ").append(std::to_string(header.timestamp)).append("\n").c_str());
			printf(std::string("Bits: ").append(std::to_string(header.bits)).append("\n").c_str());
			printf(std::string("Nonce: ").append(std::to_string(header.nonce)).append("\n").c_str());
		}
	}
}