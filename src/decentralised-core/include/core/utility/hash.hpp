#ifndef DC_CORE_SHA_HPP
#define DC_CORE_SHA_HPP

#include <cstdint>
#include <core/types.hpp>

namespace decentralised
{
	namespace core
	{
		/**
		* Generate a ripemd160 hash. This hash function is used in script for
		* op_ripemd160.
		*
		* ripemd160(data)
		*/
		short_hash ripemd160_hash(const data_chunk& chunk);

		/**
		 * Generate a sha1 hash. This hash function is used in script for op_sha1.
		 *
		 * sha1(data)
		 */
		short_hash sha1_hash(const data_chunk& chunk);

		/**
		 * Generate a sha256 hash. This hash function is used in mini keys,
		 * currently only by libwallet.
		 *
		 * sha256(data)
		 */
		hash_digest sha256_hash(const data_chunk& chunk);

		/**
		 * Generate a sha256 hash. This hash function is used in electrum seed
		 * stretching, currently only by libwallet.
		 *
		 * sha256(data)
		 */
		hash_digest sha256_hash(const data_chunk& first_chunk,
			const data_chunk& second_chunk);

		/**
		 * Generate a sha512 hash. This hash function is used in bip32 keys,
		 * currently only by libwallet.
		 *
		 * sha512(data)
		 */
		long_hash sha512_hash(const data_chunk& chunk);

		/**
		* Generate a hmac sha512 hash. This hash function is used in bip32 keys,
		* currently only by libwallet.
		*
		* hmac-sha512(data, key)
		*/
		long_hash hmac_sha512_hash(const data_chunk& chunk,
			const data_chunk& key);

		/**
		 * Generate a typical bitcoin hash. This is the most widely used
		 * hash function in Bitcoin.
		 *
		 * sha256(sha256(data))
		 */
		hash_digest bitcoin_hash(const data_chunk& chunk);

		/**
		* Generate a bitcoin short hash. This hash function is used in a
		* few specific cases where short hashes are desired.
		*
		* ripemd(sha256(data))
		*/
		short_hash bitcoin_short_hash(const data_chunk& chunk);

		/**
		 * Generate a bitcoin hash checksum. Last 4 bytes of sha256(sha256(data))
		 *
		 * int(sha256(sha256(data))[-4:])
		 */
		uint32_t bitcoin_checksum(const data_chunk& chunk);
	}
}
#endif

