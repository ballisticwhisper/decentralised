#ifndef DC_CORE_BLOCKCHAIN_LEVELDB_ORGANIZER_H
#define DC_CORE_BLOCKCHAIN_LEVELDB_ORGANIZER_H

#include <core/blockchain/organizer.hpp>

#include "leveldb_common.hpp"
#include "leveldb_chain_keeper.hpp"

namespace decentralised
{
	namespace core
	{
		class leveldb_organizer
			: public organizer
		{
		public:
			typedef blockchain::reorganize_handler reorganize_handler;

			leveldb_organizer(leveldb_common_ptr common, orphans_pool_ptr orphans,
				leveldb_chain_keeper_ptr chain, reorganize_handler handler);

		protected:
			std::error_code verify(size_t fork_index,
				const block_detail_list& orphan_chain, size_t orphan_index);
			void reorganize_occured(
				size_t fork_point,
				const blockchain::block_list& arrivals,
				const blockchain::block_list& replaced);

		private:
			leveldb_common_ptr common_;
			reorganize_handler handler_;
		};

	}
}
#endif