#include <core/network/p2p_network.hpp>

namespace decentralised
{
	namespace core
	{
		p2p_network::p2p_network(p2p_network_type type) : type_(type)
		{
		}

		p2p_network::~p2p_network()
		{
		}

		uint32_t p2p_network::get_port()
		{
			switch (type_)
			{
			case p2p_network_type::bitcoin_main:
				return bc_main_protocol_port;
			case p2p_network_type::bitcoin_testnet:
				return bc_test_protocol_port;
			default: // default to dc network
				return dc_main_protocol_port;
			}
		}

		std::vector<std::string> p2p_network::get_dns_seeds()
		{
			switch (type_)
			{
			case p2p_network_type::bitcoin_main:
				return bc_main_dns_seeds;
			case p2p_network_type::bitcoin_testnet:
				return bc_test_dns_seeds;
			default: // default to dc network
				return dc_main_dns_seeds;
			}
		}
	}
}