#include "core/decentralised_client.hpp"

namespace decentralised
{
	namespace core
	{
		decentralised_client::decentralised_client(signal_handler eventHandler) : 
			disk_pool(4),
			net_pool(1),
			mem_pool(1),
			chain(disk_pool),
			net(net_pool),
			eventHandler_(eventHandler)
		{
		}

		decentralised_client::~decentralised_client()
		{
			disk_pool.stop();
			disk_pool.join();

			net_pool.stop();
			net_pool.join();

			mem_pool.stop();
			mem_pool.join();

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

		void decentralised_client::start(const char prefix[])
		{
			std::string dbPath = std::string(prefix);

			auto handle_blockchain_started =
				std::bind(&decentralised_client::blockchain_started, this, _1);

			chain.start(dbPath, handle_blockchain_started);
		}


		void decentralised_client::blockchain_started(const std::error_code& ec)
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
		}

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