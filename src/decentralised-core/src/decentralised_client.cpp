#include "core/decentralised_client.hpp"

namespace decentralised
{
	namespace core
	{
		decentralised_client::decentralised_client() : disk_pool(4), net_pool(2)
		{

		}

		decentralised_client::~decentralised_client()
		{
			disk_pool.stop();
			net_pool.stop();

			disk_pool.join();
			net_pool.join();
		}

		leveldb::Options decentralised_client::create_open_options()
		{
			leveldb::Options options;
			//// Open LevelDB databases
			//const size_t cache_size = 1 << 20;
			//// block_cache, filter_policy and comparator must be deleted after use!
			////options.block_cache = leveldb::NewLRUCache(cache_size / 2);
			//options.write_buffer_size = cache_size / 4;
			////options.filter_policy = leveldb::NewBloomFilterPolicy(10);
			//options.compression = leveldb::kNoCompression;
			//options.max_open_files = 256;
			//options.create_if_missing = true;
			return options;
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

			return message;
		}

		void decentralised_client::start(const char prefix[])
		{
			//const size_t history_height = 0;
			//const auto genesis = genesis_block();

			// TODO: for debugging - remove
			//boost::filesystem::remove_all(prefix);

			//leveldb::Options options = create_open_options();
			//leveldb::DB* db = nullptr;
			//leveldb::Status status = leveldb::DB::Open(options, prefix, &db);

			//leveldb::Slice value_slice(reinterpret_cast<const char*>(&genesis));
			//db->Put(leveldb::WriteOptions(), "0", value_slice);

			//std::string fromDb;
			//db->Get(leveldb::ReadOptions(), "0", &fromDb);

			//libbitcoin::block_type* fromDbBlock = reinterpret_cast<libbitcoin::block_type*>(fromDb.c_str(), fromDb.size());



			//delete fromDbBlock;
			//delete db;
		}
	}
}