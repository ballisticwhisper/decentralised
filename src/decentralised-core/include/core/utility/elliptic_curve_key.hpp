#ifndef DC_CORE_ELLIPTIC_CURVE_KEY_HPP
#define DC_CORE_ELLIPTIC_CURVE_KEY_HPP

#include <openssl/ec.h>
#include <stdexcept>
#include <core/types.hpp>

namespace decentralised
{
	namespace core
	{
		typedef data_chunk private_data;
		typedef hash_digest secret_parameter;

		class elliptic_curve_key
		{
		public:
			elliptic_curve_key();
			~elliptic_curve_key();

			elliptic_curve_key(const elliptic_curve_key& other);
			elliptic_curve_key& operator=(const elliptic_curve_key& other);

			bool new_keypair(bool compressed = true);
			bool set_secret(
				const secret_parameter& secret, bool compressed = true);
			secret_parameter secret() const;
			data_chunk sign(hash_digest hash) const;

			bool set_public_key(const data_chunk& pubkey);
			data_chunk public_key() const;
			bool verify(hash_digest hash, const data_chunk& signature);

			void set_compressed(bool compressed);

		private:
			bool initialize();
			void use_compressed();

			EC_KEY* key_;
		};
	}
}

#endif

