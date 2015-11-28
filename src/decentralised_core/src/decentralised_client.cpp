#include "decentralised_client.h"

namespace decentralised
{
	namespace core
	{
		decentralised_client::decentralised_client() : disk_pool(4), net_pool(2)
		{

		}

		decentralised_client::~decentralised_client()
		{
			disk_pool.stop();
			net_pool.stop();

			disk_pool.join();
			net_pool.join();
		}

		std::string decentralised_client::get_genesis_message()
		{
			block_type blk = genesis_block();
			const transaction_type& coinbase_tx = blk.transactions[0];
			const transaction_input_type& coinbase_input = coinbase_tx.inputs[0];
			const script_type& input_script = coinbase_input.script;
			const data_chunk& raw_block_message = save_script(input_script);
			std::string message;
			message.resize(raw_block_message.size());
			std::copy(raw_block_message.begin() + 8, raw_block_message.end(),
				message.begin());

			return message;
		}
	}
}