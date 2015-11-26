#include "manager_network.h"

namespace decentralised
{
	namespace managers
	{
		manager_network::manager_network(void) : disk_pool(4), net_pool(2)
		{
		}

		manager_network::~manager_network()
		{
		}

		void manager_network::initialize()
		{
			block_type blk = genesis_block();
			printf(encode_hex(hash_block_header(blk.header)).c_str());
		}
	}
}
