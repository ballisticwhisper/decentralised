#ifndef DC_CORE_P2P_NETWORK_HPP
#define DC_CORE_P2P_NETWORK_HPP

#include <cstdint>
#include <vector>
#include <core/utility/big_number.hpp>

namespace decentralised
{
	namespace core
	{
		constexpr uint32_t bc_main_protocol_port = 18333;
		constexpr uint32_t bc_test_protocol_port = 8333;
		constexpr uint32_t dc_main_protocol_port = 8456;

		const std::vector<std::string> bc_main_dns_seeds
		{
			"bitseed.xf2.org",
			"dnsseed.bluematt.me",
			"seed.bitcoin.sipa.be",
			"dnsseed.bitcoin.dashjr.org",
			"archivum.info",
			"progressbar.sk",
			"faucet.bitcoin.st",
			"bitcoin.securepayment.cc",
			"seed.bitnodes.io"
		};

		const std::vector<std::string> bc_test_dns_seeds
		{
			"testnet-seed.bitcoin.petertodd.org",
			"testnet-seed.bluematt.me"
		};

		const std::vector<std::string> dc_main_dns_seeds
		{
			"dnsseed.decentralised-project.org"
		};

		enum p2p_network_type {
			bitcoin_main = 1,
			bitcoin_testnet,
			decentralised_main
		};

		class p2p_network {
		public:
			p2p_network(p2p_network_type type);
			~p2p_network();

			uint32_t get_port();

			std::vector<std::string> get_dns_seeds();
		private:
			p2p_network_type& type_;
		};
	}
}

#endif