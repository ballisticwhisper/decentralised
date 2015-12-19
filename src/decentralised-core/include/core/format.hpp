#ifndef DC_CORE_FORMAT_HPP
#define DC_CORE_FORMAT_HPP

#include <type_traits>
#include <boost/range/iterator_range.hpp>
#include <boost/range/sub_range.hpp>
#include <boost/detail/endian.hpp>
#include <core/constants.hpp>
#include <core/types.hpp>
#include <core/utility/assert.hpp>

namespace decentralised
{
	namespace core
	{
		// Forwarding definitions because primitives.hpp depends on this header.
		struct output_point;
		// We don't need a stream operator for input_point:
		typedef output_point input_point;

		// Extend the data by appending other.
		template <typename D, typename T>
		void extend_data(D& data, const T& other);

		template <typename T, typename Iterator>
		T from_big_endian(Iterator in);

		template <typename T, typename Iterator>
		T from_little_endian(Iterator in);

		template <typename T>
		byte_array<sizeof(T)> to_big_endian(T n);

		template <typename T>
		byte_array<sizeof(T)> to_little_endian(T n);

		template <typename T>
		std::string encode_hex(T data);

		// ADL cannot work on templates
		std::ostream& operator<<(
			std::ostream& stream, const data_chunk& data);
		std::ostream& operator<<(
			std::ostream& stream, const hash_digest& hash);
		std::ostream& operator<<(
			std::ostream& stream, const short_hash& hash);
		std::ostream& operator<<(
			std::ostream& stream, const output_point& point);

		data_chunk decode_hex(std::string hex);

		/**
		 * Convert a hex string into hash bytes.
		 * On error, returns null_hash.
		 */
		hash_digest decode_hash(const std::string& hex);

		/**
		 * Convert a hex string into short hash bytes.
		 * On error, returns null_short_hash.
		 */
		short_hash decode_short_hash(const std::string& hex);

		std::string satoshi_to_btc(uint64_t value);

	}
}

#include <core/impl/format.ipp>

#endif

