#ifndef DC_CORE_ADDRESS_HPP
#define DC_CORE_ADDRESS_HPP

#include <core/constants.hpp>
#include <core/types.hpp>
#include <core/script.hpp>

namespace decentralised
{
	namespace core
	{
		/**
		 * A class for handling Bitcoin addresses. Supports encoding and decoding
		 * Bitcoin string addresses. (taken from libbitcoin version 1 branch)
		 *
		 * To validate a Bitcoin address we can try to set a string address.
		 *
		 * @code
		 *   payment_address payaddr;
		 *   if (!payaddr.set_encoded("155GwFbFET2HCT6r6jHAHUoxc897sSdjaq"))
		 *       // Address is invalid
		 * @endcode
		 *
		 * To check whether a payment_address has successfully been set, the
		 * hash value can be compared to null_short_hash (defined in constants.hpp).
		 *
		 * @code
		 *   if (payaddr.version() == payment_address::invalid_version)
		 *       // This payment_address is empty.
		 * @endcode
		 */
		class payment_address
		{
		public:
#ifdef ENABLE_TESTNET
			enum
			{
				pubkey_version = 0x6f,
				script_version = 0xc4,
				wif_version = 0xef,
				invalid_version = 0xff
			};
#else
			enum
			{
				pubkey_version = 0x00,
				script_version = 0x05,
				wif_version = 0x80,
				invalid_version = 0xff
			};
#endif
			payment_address();
			payment_address(uint8_t version, const short_hash& hash);
			payment_address(const std::string& encoded_address);

			void set(uint8_t version, const short_hash& hash);

			uint8_t version() const;
			const short_hash& hash() const;

			bool set_encoded(const std::string& encoded_address);
			std::string encoded() const;

		private:
			uint8_t version_ = invalid_version;
			short_hash hash_ = null_short_hash;
		};

		void set_public_key_hash(payment_address& address,
			const short_hash& pubkey_hash);
		void set_script_hash(payment_address& address,
			const short_hash& script_hash);

		void set_public_key(payment_address& address,
			const data_chunk& public_key);
		void set_script(payment_address& address,
			const script_type& eval_script);

		/**
		 * Extract a Bitcoin address from an input or output script.
		 * Returns false on failure.
		 */
		bool extract(payment_address& address, const script_type& script);

		bool operator==(const payment_address& lhs, const payment_address& rhs);

	}
}

// Allow payment_address to be in indexed in std::*map classes.
namespace std
{
    template <>
    struct hash<decentralised::core::payment_address>
    {
		size_t operator()(const decentralised::core::payment_address& payaddr) const
        {
			using decentralised::core::short_hash;
			using decentralised::core::short_hash_size;
            std::string raw_addr;
            raw_addr.resize(short_hash_size + 1);
            raw_addr[0] = payaddr.version();
            const short_hash& addr_hash = payaddr.hash();
            std::copy(addr_hash.begin(), addr_hash.end(),
                raw_addr.begin() + 1);
            std::hash<std::string> hash_fn;
            return hash_fn(raw_addr);
        }
    };
}

#endif

