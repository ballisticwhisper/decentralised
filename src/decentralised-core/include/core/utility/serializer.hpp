#ifndef DC_CORE_SERIALIZER_HPP
#define DC_CORE_SERIALIZER_HPP

#include <boost/asio/streambuf.hpp>
#include <string>
#include <vector>
#include <core/format.hpp>
#include <core/primitives.hpp>
#include <core/types.hpp>

namespace decentralised
{
	namespace core
	{
		size_t variable_uint_size(uint64_t v);

		/**
		 * Serializer that uses iterators and is oblivious to the underlying
		 * container type. Is not threadsafe.
		 *
		 * Use the helper make_serializer() to construct a serializer without
		 * needing to specify the Iterator type.
		 *
		 * Makes no assumptions about the size of the underlying container type.
		 * User is responsible for allocating enough space prior to serialization.
		 *
		 * @code
		 *  data_chunk buffer(8);
		 *  auto serial = make_serializer(buffer.begin());
		 *  serial.write_8_bytes(110);
		 * @endcode
		 */
		template <typename Iterator>
		class serializer
		{
		public:
			serializer(const Iterator begin);

			/* These write data in little endian format: */
			void write_byte(uint8_t value);
			void write_2_bytes(uint16_t value);
			void write_4_bytes(uint32_t value);
			void write_8_bytes(uint64_t value);

			/**
			 * Encodes an unsigned integer in big-endian format.
			 */
			template <typename T>
			void write_big_endian(T n);

			/**
			 * Encodes an unsigned integer in little-endian format.
			 */
			template <typename T>
			void write_little_endian(T n);

			/**
			 * Variable uints are usually used for sizes.
			 * They're encoded using fewer bytes depending on the value itself.
			 */
			void write_variable_uint(uint64_t value);

			template <typename T>
			void write_data(const T& data);
			void write_hash(const hash_digest& hash);
			void write_short_hash(const short_hash& hash);

			void write_network_address(network_address_type addr);

			/**
			 * Write a fixed size string padded with zeroes.
			 */
			void write_fixed_string(const std::string& command, size_t string_size);

			/**
			 * Write a variable length string.
			 */
			void write_string(const std::string& str);

			/**
			 * Returns underlying iterator.
			 */
			Iterator iterator();

			/**
			 * Useful if you want to serialize some data using another
			 * routine and then continue with this serializer.
			 */
			void set_iterator(Iterator iter);

		private:
			template <typename T>
			void write_data_reverse(const T& data);

			// We need to advance the internal iterator.
			// std::copy gives no info on length of the data copied.
			template <typename InputIterator>
			void internal_copy(InputIterator first, InputIterator last);

			Iterator iter_;
		};

		template <typename Iterator>
		serializer<Iterator> make_serializer(Iterator begin);

		class end_of_stream
			: std::exception {};

		/**
		 * Deserializer that uses iterators and is oblivious to the underlying
		 * container type. Is not threadsafe.
		 *
		 * Use the helper make_deserializer() to construct a deserializer without
		 * needing to specify the Iterator type.
		 *
		 * Throws end_of_stream exception upon early termination during deserialize.
		 * For example, calling read_8_bytes() with only 5 bytes remaining will throw.
		 *
		 * @code
		 *  auto deserial = make_deserializer(data.begin(), data.end());
		 *  try {
		 *    uint64_t value = deserial.read_8_bytes();
		 *  } catch (end_of_stream) {
		 *    // ...
		 *  }
		 * @endcode
		 */
		template <typename Iterator>
		class deserializer
		{
		public:
			deserializer(const Iterator begin, const Iterator end);

			/* These read data in little endian format: */
			uint8_t read_byte();
			uint16_t read_2_bytes();
			uint32_t read_4_bytes();
			uint64_t read_8_bytes();

			/**
			 * Reads an unsigned integer that has been encoded in big endian format.
			 */
			template <typename T>
			T read_big_endian();

			/**
			 * Reads an unsigned integer that has been encoded in little endian format.
			 */
			template <typename T>
			T read_little_endian();

			/**
			 * Variable uints are usually used for sizes.
			 * They're encoded using fewer bytes depending on the value itself.
			 */
			uint64_t read_variable_uint();

			data_chunk read_data(uint32_t n_bytes);
			hash_digest read_hash();
			short_hash read_short_hash();

			network_address_type read_network_address();

			/**
			 * Read a fixed size string padded with zeroes.
			 */
			std::string read_fixed_string(size_t len);

			/**
			 * Read a variable length string.
			 */
			std::string read_string();

			/**
			 * Read a fixed-length data block.
			 */
			template<unsigned N>
			byte_array<N> read_bytes();

			template<unsigned N>
			byte_array<N> read_bytes_reverse();

			/**
			 * Returns underlying iterator.
			 */
			Iterator iterator() const;

			/**
			 * Useful if you advance the iterator using other serialization
			 * methods or objects.
			 */
			void set_iterator(const Iterator iter);

		private:
			// Try to advance iterator 'distance' incremenets forwards.
			// Throw if we prematurely reach the end.
			static void check_distance(
				Iterator it, const Iterator end, size_t distance);

			Iterator iter_;
			const Iterator end_;
		};

		template <typename Iterator>
		deserializer<Iterator> make_deserializer(
			const Iterator begin, const Iterator end);

	}
}
#include <core/impl/utility/serializer.ipp>

#endif

