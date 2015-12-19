#ifndef DC_CORE_CHECKSUM_HPP
#define DC_CORE_CHECKSUM_HPP

#include <core/types.hpp>

namespace decentralised
{
	namespace core
	{
		/**
		 * Appends a four-byte checksum to a data chunk.
		 */
		void append_checksum(data_chunk& data);

		/**
		 * Verifies the last four bytes of a data chunk are a valid checksum of the
		 * earlier bytes. This is typically used to verify base58 data.
		 */
		bool verify_checksum(const data_chunk& data);

	}
}
#endif

