#include "core/decentralised_client.hpp"

namespace decentralised
{
	namespace core
	{
		decentralised_client::decentralised_client()
		{

		}

		decentralised_client::~decentralised_client()
		{
			if (chain)
				delete chain;

			if (pool)
				delete pool;
		}

		std::string decentralised_client::get_genesis_message()
		{
			block_type blk = genesis_block();
			const transaction_type& coinbase_tx = blk.transactions[0];
			const transaction_input_type& coinbase_input = coinbase_tx.inputs[0];
			const script_type& input_script = coinbase_input.script;
			const data_chunk& raw_block_message = save_script(input_script);
			std::string message;
			message.resize(raw_block_message.size());
			std::copy(raw_block_message.begin() + 8, raw_block_message.end(),
				message.begin());

			return message.append("\n");
		}

		void decentralised_client::start(const char prefix[])
		{
			std::string dbPath = std::string(prefix);

			//create_stealth_db(dbPath + "\\stealth.db");

			pool = new threadpool(1);
			chain = new leveldb_blockchain(*pool);
			auto blockchain_start = [](const std::error_code& ec) {};

			chain->start(dbPath, blockchain_start);

			block_type first_block = genesis_block();

			std::promise<std::error_code> ec_promise;
			auto import_finished =
				[&ec_promise](const std::error_code& ec)
			{
				ec_promise.set_value(ec);
			};
			chain->import(first_block, 0, import_finished);

			std::error_code ec = ec_promise.get_future().get();
			if (ec)
			{
				printf("Error importing genesis block");
				//log_error() << "Importing genesis block failed: " << ec.message();
				//return -1;
			}
			else
			{
				printf("\nImported genesis block");
				//			log_info() << "Imported genesis block "
				//<< hash_block_header(first_block.header);
				// All threadpools stopping in parallel...
				pool->stop();
				// ... Make them all join main thread and wait until they finish.
				pool->join();
				// Now safely close leveldb_blockchain.
				chain->stop();
			}
		}

		void decentralised_client::create_file(const std::string& filename, size_t filesize)
		{
			std::ofstream file(filename, std::ios::binary | std::ios::trunc);
			constexpr size_t chunk_size = 100000;
			std::vector<char> random_buffer(chunk_size);
			for (size_t i = 0; i < filesize; i += chunk_size)
				file.write(random_buffer.data(), chunk_size);
		}

		void decentralised_client::create_stealth_db(const std::string &filename)
		{
			create_file(filename, 100000000);
			mmfile file(filename);
			auto serial = make_serializer(file.data());
			serial.write_4_bytes(1);
			// should last us a decade
			size_t max_header_rows = 10000;
			serial.write_4_bytes(max_header_rows);
			serial.write_4_bytes(0);
			for (size_t i = 0; i < max_header_rows; ++i)
				serial.write_4_bytes(0);
		}
	}
}