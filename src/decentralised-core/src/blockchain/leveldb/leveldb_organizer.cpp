#include "leveldb_organizer.hpp"

#include <core/utility/assert.hpp>

#include "leveldb_validate_block.hpp"

namespace decentralised
{
	namespace core
	{
		leveldb_organizer::leveldb_organizer(leveldb_common_ptr common,
			orphans_pool_ptr orphans, leveldb_chain_keeper_ptr chain,
			reorganize_handler handler)
			: organizer(orphans, chain), common_(common), handler_(handler)
		{
		}

		std::error_code leveldb_organizer::verify(size_t fork_index,
			const block_detail_list& orphan_chain, size_t orphan_index)
		{
			BITCOIN_ASSERT(orphan_index < orphan_chain.size());
			const block_type& current_block = orphan_chain[orphan_index]->actual();
			size_t height = fork_index + orphan_index + 1;
			BITCOIN_ASSERT(height != 0);
			leveldb_validate_block validate(common_, fork_index, orphan_chain,
				orphan_index, height, current_block);
			// Perform checks.
			std::error_code ec;
			ec = validate.check_block();
			if (ec)
				return ec;
			ec = validate.accept_block();
			if (ec)
				return ec;
			// Skip non-essential checks if before last checkpoint.
			if (fork_index < 278702)
				return std::error_code();
			// Perform strict but slow tests - connect_block()
			return validate.connect_block();
		}
		void leveldb_organizer::reorganize_occured(
			size_t fork_point,
			const blockchain::block_list& arrivals,
			const blockchain::block_list& replaced)
		{
			handler_(std::error_code(), fork_point, arrivals, replaced);
		}

	}
}