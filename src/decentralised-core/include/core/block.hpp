#ifndef DC_CORE_BLOCK_HPP
#define DC_CORE_BLOCK_HPP

#include <core/primitives.hpp>
#include <core/utility/big_number.hpp>

namespace decentralised
{
	namespace core
	{
		enum class block_status
		{
			orphan,
			confirmed,
			rejected
		};

		struct block_info
		{
			block_status status;
			size_t height;
		};

		bool operator==(
			const block_header_type& block_a, const block_header_type& block_b);

		uint64_t block_value(size_t height);
		big_number block_work(uint32_t bits);

		hash_digest hash_block_header(const block_header_type& header);

		index_list block_locator_indexes(int top_height);

		block_type genesis_block();
	}
}
#endif

