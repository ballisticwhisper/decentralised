#ifndef DC_CORE_STEALTH_HPP
#define DC_CORE_STEALTH_HPP

#include <core/types.hpp>

namespace decentralised
{
	namespace core
	{
		typedef uint32_t stealth_bitfield;

		struct stealth_prefix
		{
			uint8_t number_bits;
			stealth_bitfield bitfield;
		};

		bool stealth_match(
			const stealth_prefix& prefix, const uint8_t* raw_bitfield);

		stealth_bitfield calculate_stealth_bitfield(
			const data_chunk& stealth_data);

	}
}
#endif

