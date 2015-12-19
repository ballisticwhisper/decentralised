#ifndef DC_CORE_BASE58_HPP
#define DC_CORE_BASE58_HPP

#include <core/types.hpp>

namespace decentralised
{
	namespace core
	{
		bool is_base58(const char c);
		bool is_base58(const std::string& text);

		std::string encode_base58(const data_chunk& unencoded);

		data_chunk decode_base58(std::string encoded);
	}
}

#endif

