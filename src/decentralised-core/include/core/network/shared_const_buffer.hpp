#ifndef DC_CORE_NET_SHARED_CONST_BUFFER_HPP
#define DC_CORE_NET_SHARED_CONST_BUFFER_HPP

#include <boost/asio.hpp>

#include <core/types.hpp>
#include <core/utility/serializer.hpp>

namespace decentralised
{
	namespace core
	{
		// A reference-counted non-modifiable buffer class.
		class shared_const_buffer
		{
		public:
			// Construct from a stream object
			explicit shared_const_buffer(const data_chunk& user_data)
				: data_(std::make_shared<data_chunk>(
				std::begin(user_data), std::end(user_data))),
				buffer_(boost::asio::buffer(*data_))
			{
			}

			// Implement the ConstBufferSequence requirements.
			typedef boost::asio::const_buffer value_type;
			typedef const value_type* const_iterator;

			const_iterator begin() const
			{
				return &buffer_;
			}
			const_iterator end() const
			{
				return &buffer_ + 1;
			}

		private:
			std::shared_ptr<data_chunk> data_;
			value_type buffer_;
		};

	}
}
#endif

