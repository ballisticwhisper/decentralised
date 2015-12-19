#include <core/utility/checksum.hpp>

#include <core/format.hpp>
#include <core/utility/hash.hpp>

namespace decentralised
{
	namespace core
	{
		void append_checksum(data_chunk& data)
		{
			uint32_t checksum = bitcoin_checksum(data);
			extend_data(data, to_little_endian(checksum));
		}

		bool verify_checksum(const data_chunk& data)
		{
			data_chunk body(data.begin(), data.end() - 4);
			auto checksum = from_little_endian<uint32_t>(data.end() - 4);
			return bitcoin_checksum(body) == checksum;
		}

	}
}