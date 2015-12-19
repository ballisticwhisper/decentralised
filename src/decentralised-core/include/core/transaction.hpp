#ifndef DC_CORE_TRANSACTION_HPP
#define DC_CORE_TRANSACTION_HPP

#include <core/primitives.hpp>

namespace decentralised
{
	namespace core
	{
		typedef std::vector<uint64_t> output_value_list;

		// Used by transaction_indexer
		struct output_info_type
		{
			output_point point;
			uint64_t value;
		};

		typedef std::vector<output_info_type> output_info_list;

		hash_digest hash_transaction(const transaction_type& tx);
		// hash_type_code is used by OP_CHECKSIG
		hash_digest hash_transaction(const transaction_type& tx,
			uint32_t hash_type_code);

		hash_digest generate_merkle_root(const transaction_list& transactions);

		std::string pretty(const transaction_type& transaction);

		bool previous_output_is_null(const output_point& previous_output);
		bool is_coinbase(const transaction_type& tx);

		uint64_t total_output_value(const transaction_type& tx);

		bool operator==(
			const output_point& output_a, const output_point& output_b);
		bool operator!=(
			const output_point& output_a, const output_point& output_b);

		bool is_final(const transaction_type& tx,
			size_t block_height, uint32_t block_time);

	}
}
#endif

