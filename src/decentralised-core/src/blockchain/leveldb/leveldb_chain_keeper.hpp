#ifndef DC_CORE_BLOCKCHAIN_LEVELDB_CHAIN_KEEPER_H
#define DC_CORE_BLOCKCHAIN_LEVELDB_CHAIN_KEEPER_H

#include <core/blockchain/organizer.hpp>
#include <core/blockchain/leveldb_blockchain.hpp>

#include "leveldb_common.hpp"

namespace decentralised
{
	namespace core
	{
		class leveldb_chain_keeper
			: public chain_keeper
		{
		public:
			leveldb_chain_keeper(leveldb_common_ptr common, leveldb_databases db);

			void start();
			void stop();

			void add(block_detail_ptr incoming_block);
			int find_index(const hash_digest& search_block_hash);
			big_number end_slice_difficulty(size_t slice_begin_index);
			bool end_slice(size_t slice_begin_index,
				block_detail_list& sliced_blocks);

		private:
			bool clear_transaction_data(leveldb_transaction_batch& batch,
				const transaction_type& remove_tx);

			leveldb_common_ptr common_;
			leveldb_databases db_;
		};

		typedef std::shared_ptr<leveldb_chain_keeper> leveldb_chain_keeper_ptr;

	}
}
#endif