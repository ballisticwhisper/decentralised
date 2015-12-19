#ifndef DC_CORE_BIG_NUMBER_HPP
#define DC_CORE_BIG_NUMBER_HPP

#include <openssl/bn.h>
#include <core/types.hpp>

namespace decentralised
{
	namespace core
	{
		class big_number;
		typedef std::pair<big_number, big_number> divmod_result;

		// TODO: Lots of things *should* throw on error in this class
		class big_number
		{
		public:
			big_number();
			big_number(uint32_t value);
			big_number(const big_number& other);
			~big_number();

			big_number& operator=(const big_number& other);

			void set_compact(uint32_t compact);
			uint32_t compact() const;

			void set_data(data_chunk load_data);
			data_chunk data() const;

			void set_hash(hash_digest load_hash);
			hash_digest hash() const;

			void set_uint32(uint32_t value);
			uint32_t uint32() const;

			void set_int32(int32_t value);
			int32_t int32() const;

			// Used by script system
			void set_uint64(uint64_t value);
			void set_int64(int64_t value);

			bool operator==(const big_number& other);
			bool operator!=(const big_number& other);
			bool operator<=(const big_number& other);
			bool operator>=(const big_number& other);
			bool operator<(const big_number& other);
			bool operator>(const big_number& other);

			big_number& operator+=(const big_number& other);
			big_number& operator-=(const big_number& other);
			big_number& operator*=(const big_number& other);
			big_number& operator/=(const big_number& other);

		private:
			friend divmod_result divmod(const big_number& a, const big_number& b);
			friend const big_number operator+(
				const big_number& a, const big_number& b);
			friend const big_number operator-(
				const big_number& a, const big_number& b);
			friend const big_number operator-(const big_number& number);
			friend const big_number operator/(const big_number& a,
				const big_number& b);
			friend const big_number operator<<(const big_number& a,
				unsigned int shift);

			void initialize();
			void copy(const big_number& other);

			void set_uint64_impl(uint64_t value, bool is_negative);

			BIGNUM bignum_;
		};

		const big_number operator+(const big_number& a, const big_number& b);
		const big_number operator-(const big_number& a, const big_number& b);
		const big_number operator-(const big_number& number);
		const big_number operator/(const big_number& a, const big_number& b);
		const big_number operator<<(const big_number& a, unsigned int shift);

		divmod_result divmod(const big_number& a, const big_number& b);

	}
}
#endif

