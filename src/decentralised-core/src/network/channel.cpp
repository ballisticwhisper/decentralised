#include <core/network/channel.hpp>
#include <core/utility/hash.hpp>

namespace decentralised
{
	namespace core
	{
		using std::placeholders::_1;
		using std::placeholders::_2;
		using boost::posix_time::time_duration;
		using boost::posix_time::minutes;
		using boost::posix_time::seconds;
		using boost::asio::buffer;

		// Connection timeout time
		const time_duration initial_timeout = seconds(0) + minutes(1);
		const time_duration disconnect_timeout = seconds(0) + minutes(90);
		const time_duration heartbeat_time = seconds(0) + minutes(30);

		template <typename Message>
		class channel_loader_module
			: public channel_loader_module_base
		{
		public:
			typedef std::function<
				void(const std::error_code&, const Message&)> load_handler;

			channel_loader_module(load_handler handle_load)
				: handle_load_(handle_load) {}

			void attempt_load(const data_chunk& stream) const
			{
				Message result;
				try
				{
					satoshi_load(stream.begin(), stream.end(), result);
					handle_load_(std::error_code(), result);
				}
				catch (end_of_stream)
				{
					handle_load_(error::bad_stream, Message());
				}
			}

			const std::string lookup_symbol() const
			{
				return satoshi_command(Message());
			}
		private:
			load_handler handle_load_;
		};

		channel_stream_loader::~channel_stream_loader()
		{
			for (channel_loader_module_base* module : modules_)
				delete module;
		}

		void channel_stream_loader::add(channel_loader_module_base* module)
		{
			modules_.push_back(module);
		}

		void channel_stream_loader::load_lookup(const std::string& symbol,
			const data_chunk& stream) const
		{
			for (channel_loader_module_base* module : modules_)
				if (module->lookup_symbol() == symbol)
					module->attempt_load(stream);
		}

		channel_proxy::channel_proxy(threadpool& pool, socket_ptr socket)
			: strand_(pool.service()), socket_(socket),
			timeout_(pool.service()), heartbeat_(pool.service()), stopped_(false)
		{
#define CHANNEL_TRANSPORT_MECHANISM(MESSAGE_TYPE) \
    MESSAGE_TYPE##_subscriber_ = \
        std::make_shared<MESSAGE_TYPE##_subscriber_type>(pool); \
    loader_.add(new channel_loader_module<MESSAGE_TYPE##_type>( \
        std::bind(&MESSAGE_TYPE##_subscriber_type::relay, \
            MESSAGE_TYPE##_subscriber_, _1, _2)));

			CHANNEL_TRANSPORT_MECHANISM(version);
			CHANNEL_TRANSPORT_MECHANISM(verack);
			CHANNEL_TRANSPORT_MECHANISM(address);
			CHANNEL_TRANSPORT_MECHANISM(get_address);
			CHANNEL_TRANSPORT_MECHANISM(inventory);
			CHANNEL_TRANSPORT_MECHANISM(get_data);
			CHANNEL_TRANSPORT_MECHANISM(get_blocks);
			CHANNEL_TRANSPORT_MECHANISM(transaction);
			CHANNEL_TRANSPORT_MECHANISM(block);

#undef CHANNEL_TRANSPORT_MECHANISM

			raw_subscriber_ = std::make_shared<raw_subscriber_type>(pool);
			stop_subscriber_ = std::make_shared<stop_subscriber_type>(pool);
		}

		channel_proxy::~channel_proxy()
		{
			stop();
		}

		void channel_proxy::start()
		{
			read_header();
			set_timeout(initial_timeout);
			set_heartbeat(heartbeat_time);
		}

		void channel_proxy::stop()
		{
			if (stopped_)
				return;
			stop_impl();
			stop_subscriber_->relay(error::service_stopped);
		}
		void channel_proxy::stop_impl()
		{
			// We need this because the timeout timer shares this code with stop()
			// But sends a different error_code
			stopped_ = true;
			// Ignore the error_code. We don't really care at this point.
			boost::system::error_code ret_ec;
			timeout_.cancel(ret_ec);
			heartbeat_.cancel(ret_ec);
			socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ret_ec);
			socket_->close(ret_ec);
			clear_subscriptions();
		}

		void channel_proxy::clear_subscriptions()
		{
#define CHANNEL_CLEAR_SUBSCRIPTION(MESSAGE_TYPE) \
    MESSAGE_TYPE##_subscriber_->relay(error::service_stopped, \
        MESSAGE_TYPE##_type());

			CHANNEL_CLEAR_SUBSCRIPTION(version);
			CHANNEL_CLEAR_SUBSCRIPTION(verack);
			CHANNEL_CLEAR_SUBSCRIPTION(address);
			CHANNEL_CLEAR_SUBSCRIPTION(get_address);
			CHANNEL_CLEAR_SUBSCRIPTION(inventory);
			CHANNEL_CLEAR_SUBSCRIPTION(get_data);
			CHANNEL_CLEAR_SUBSCRIPTION(get_blocks);
			CHANNEL_CLEAR_SUBSCRIPTION(transaction);
			CHANNEL_CLEAR_SUBSCRIPTION(block);

#undef CHANNEL_CLEAR_SUBSCRIPTION

			raw_subscriber_->relay(error::service_stopped,
				header_type(), data_chunk());
		}

		bool channel_proxy::stopped() const
		{
			return stopped_;
		}

		bool timer_errors(const boost::system::error_code& ec, bool stopped)
		{
			if (ec == boost::asio::error::operation_aborted)
				return true;
			else if (ec)
			{
				log_error(LOG_NETWORK) << ec.message();
				return true;
			}
			else if (stopped)
				return true;
			return false;
		}

		void channel_proxy::handle_timeout(const boost::system::error_code& ec)
		{
			if (timer_errors(ec, stopped_))
				return;
			log_debug(LOG_NETWORK) << "Forcing disconnect due to timeout.";
			// No response for a while so disconnect
			boost::system::error_code ret_ec;
			tcp::endpoint remote_endpoint = socket_->remote_endpoint(ret_ec);
			if (!ec)
				log_debug(LOG_NETWORK) << "Closing channel "
				<< remote_endpoint.address().to_string();
			ret_ec = boost::system::error_code();
			// Force the socket closed
			// Should we do something with these error_codes?
			socket_->shutdown(tcp::socket::shutdown_both, ret_ec);
			socket_->close(ret_ec);
			stop_impl();
			stop_subscriber_->relay(error::channel_timeout);
		}

		void handle_ping(const std::error_code&)
		{
			// if there's a problem sending then this channel will be stopped
		}
		void channel_proxy::handle_heartbeat(const boost::system::error_code& ec)
		{
			if (timer_errors(ec, stopped_))
				return;
			send(ping_type(), handle_ping);
		}

		void channel_proxy::set_timeout(const time_duration timeout)
		{
			timeout_.cancel();
			timeout_.expires_from_now(timeout);
			timeout_.async_wait(std::bind(
				&channel_proxy::handle_timeout, shared_from_this(), _1));
		}
		void channel_proxy::set_heartbeat(const time_duration timeout)
		{
			heartbeat_.cancel();
			heartbeat_.expires_from_now(timeout);
			heartbeat_.async_wait(std::bind(
				&channel_proxy::handle_heartbeat, shared_from_this(), _1));
		}
		void channel_proxy::reset_timers()
		{
			set_timeout(disconnect_timeout);
			set_heartbeat(heartbeat_time);
		}

		bool channel_proxy::problems_check(const boost::system::error_code& ec)
		{
			if (stopped_)
				return true;
			else if (!ec)
				return false;
			stop();
			return true;
		}

		void channel_proxy::read_header()
		{
			async_read(*socket_, buffer(inbound_header_),
				strand_.wrap(std::bind(&channel_proxy::handle_read_header,
				shared_from_this(), _1, _2)));
		}

		void channel_proxy::read_checksum(const header_type& header_msg)
		{
			async_read(*socket_, buffer(inbound_checksum_),
				strand_.wrap(std::bind(&channel_proxy::handle_read_checksum,
				shared_from_this(), _1, _2, header_msg)));
		}

		void channel_proxy::read_payload(const header_type& header_msg)
		{
			inbound_payload_.resize(header_msg.payload_length);
			async_read(*socket_, buffer(inbound_payload_, header_msg.payload_length),
				strand_.wrap(std::bind(&channel_proxy::handle_read_payload,
				shared_from_this(), _1, _2, header_msg)));
		}

		bool verify_header(const header_type& header_msg)
		{
			if (header_msg.magic != magic_value())
				return false;
			if (header_msg.command == "verack"
				|| header_msg.command == "getaddr")
			{
				if (header_msg.payload_length != 0)
					return false;
			}
			else if (header_msg.command == "ping"
				|| header_msg.command == "pong")
			{
				if (header_msg.payload_length != 8)
					return false;
			}
			else if (header_msg.command == "version"
				|| header_msg.command == "inv"
				|| header_msg.command == "addr"
				|| header_msg.command == "getdata"
				|| header_msg.command == "getblocks"
				|| header_msg.command == "getheaders"
				|| header_msg.command == "tx"
				|| header_msg.command == "block"
				|| header_msg.command == "headers"
				|| header_msg.command == "alert")
			{
				// Should check if sizes make sense
				// i.e for addr should be multiple of 30x + 1 byte
				// Also then add ASSERTS to handlers above.
			}
			// Ignore unknown headers
			return true;
		}

		void channel_proxy::handle_read_header(const boost::system::error_code& ec,
			size_t bytes_transferred)
		{
			if (problems_check(ec))
				return;
			BITCOIN_ASSERT(bytes_transferred == header_chunk_size);
			data_chunk header_stream =
				data_chunk(inbound_header_.begin(), inbound_header_.end());
			BITCOIN_ASSERT(header_stream.size() == header_chunk_size);
			header_type header_msg;
			satoshi_load(header_stream.begin(), header_stream.end(), header_msg);

			if (!verify_header(header_msg))
			{
				log_debug(LOG_NETWORK) << "Bad header received.";
				stop();
				return;
			}

			log_debug(LOG_NETWORK) << "r: " << header_msg.command
				<< " (" << header_msg.payload_length << " bytes)";
			read_checksum(header_msg);
			reset_timers();
		}

		void channel_proxy::handle_read_checksum(const boost::system::error_code& ec,
			size_t bytes_transferred, header_type& header_msg)
		{
			if (problems_check(ec))
				return;
			BITCOIN_ASSERT(bytes_transferred == header_checksum_size);
			header_msg.checksum =
				from_little_endian<uint32_t>(inbound_checksum_.begin());
			read_payload(header_msg);
			reset_timers();
		}

		void channel_proxy::handle_read_payload(const boost::system::error_code& ec,
			size_t bytes_transferred, const header_type& header_msg)
		{
			if (problems_check(ec))
				return;
			BITCOIN_ASSERT(bytes_transferred == header_msg.payload_length);
			data_chunk payload_stream = data_chunk(
				inbound_payload_.begin(), inbound_payload_.end());
			BITCOIN_ASSERT(payload_stream.size() == header_msg.payload_length);
			if (header_msg.checksum != bitcoin_checksum(payload_stream))
			{
				log_warning(LOG_NETWORK) << "Bad checksum!";
				raw_subscriber_->relay(error::bad_stream,
					header_type(), data_chunk());
				stop();
				return;
			}
			raw_subscriber_->relay(std::error_code(),
				header_msg, inbound_payload_);

			// This must happen before calling subscribe notification handlers
			// In case user tries to stop() this channel.
			read_header();
			reset_timers();

			loader_.load_lookup(header_msg.command, payload_stream);
		}

		void channel_proxy::call_handle_send(const boost::system::error_code& ec,
			send_handler handle_send)
		{
			if (problems_check(ec))
				handle_send(error::service_stopped);
			else
				handle_send(std::error_code());
		}

		void channel_proxy::subscribe_version(receive_version_handler handle_receive)
		{
			generic_subscribe<version_type>(
				handle_receive, version_subscriber_);
		}
		void channel_proxy::subscribe_verack(receive_verack_handler handle_receive)
		{
			generic_subscribe<verack_type>(
				handle_receive, verack_subscriber_);
		}
		void channel_proxy::subscribe_address(receive_address_handler handle_receive)
		{
			generic_subscribe<address_type>(
				handle_receive, address_subscriber_);
		}
		void channel_proxy::subscribe_inventory(
			receive_inventory_handler handle_receive)
		{
			generic_subscribe<inventory_type>(
				handle_receive, inventory_subscriber_);
		}
		void channel_proxy::subscribe_get_data(
			receive_get_data_handler handle_receive)
		{
			generic_subscribe<get_data_type>(
				handle_receive, get_data_subscriber_);
		}
		void channel_proxy::subscribe_get_blocks(
			receive_get_blocks_handler handle_receive)
		{
			generic_subscribe<get_blocks_type>(
				handle_receive, get_blocks_subscriber_);
		}
		void channel_proxy::subscribe_transaction(
			receive_transaction_handler handle_receive)
		{
			generic_subscribe<transaction_type>(
				handle_receive, transaction_subscriber_);
		}
		void channel_proxy::subscribe_block(receive_block_handler handle_receive)
		{
			generic_subscribe<block_type>(
				handle_receive, block_subscriber_);
		}
		void channel_proxy::subscribe_get_address(
			receive_get_address_handler handle_receive)
		{
			generic_subscribe<get_address_type>(
				handle_receive, get_address_subscriber_);
		}
		void channel_proxy::subscribe_raw(receive_raw_handler handle_receive)
		{
			if (stopped_)
				handle_receive(error::service_stopped,
				header_type(), data_chunk());
			else
				raw_subscriber_->subscribe(handle_receive);
		}

		void channel_proxy::subscribe_stop(stop_handler handle_stop)
		{
			if (stopped_)
				handle_stop(error::service_stopped);
			else
				stop_subscriber_->subscribe(handle_stop);
		}

		void channel_proxy::send_raw(const header_type& packet_header,
			const data_chunk& payload, send_handler handle_send)
		{
			if (stopped_)
				handle_send(error::service_stopped);
			else
				strand_.post(std::bind(&channel_proxy::do_send_raw,
				shared_from_this(), packet_header, payload, handle_send));
		}
		void channel_proxy::do_send_raw(const header_type& packet_header,
			const data_chunk& payload, send_handler handle_send)
		{
			data_chunk raw_header(satoshi_raw_size(packet_header));
			satoshi_save(packet_header, raw_header.begin());
			// Construct completed packet with header + payload
			data_chunk whole_message = raw_header;
			extend_data(whole_message, payload);
			do_send_common(whole_message, handle_send);
		}
		void channel_proxy::do_send_common(const data_chunk& whole_message,
			send_handler handle_send)
		{
			shared_const_buffer buffer(whole_message);
			async_write(*socket_, buffer,
				std::bind(&channel_proxy::call_handle_send, shared_from_this(),
				std::placeholders::_1, handle_send));
		}

		// channel

		channel::channel(channel_proxy_ptr proxy)
		{
			weak_proxy_ = proxy;
		}
		channel::~channel()
		{
			stop();
		}

		void channel::stop()
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			// Slowly shutdown
			if (proxy)
				proxy->stop();
		}
		bool channel::stopped() const
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			// Slowly shutdown
			if (!proxy)
				return true;
			else
				return proxy->stopped();
		}

		void channel::send_raw(const header_type& packet_header,
			const data_chunk& payload, channel_proxy::send_handler handle_send)
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			if (!proxy)
				handle_send(error::service_stopped);
			else
				proxy->send_raw(packet_header, payload, handle_send);
		}

		void channel::subscribe_version(
			channel_proxy::receive_version_handler handle_receive)
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			if (!proxy)
				handle_receive(error::service_stopped, version_type());
			else
				proxy->subscribe_version(handle_receive);
		}
		void channel::subscribe_verack(
			channel_proxy::receive_verack_handler handle_receive)
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			if (!proxy)
				handle_receive(error::service_stopped, verack_type());
			else
				proxy->subscribe_verack(handle_receive);
		}
		void channel::subscribe_address(
			channel_proxy::receive_address_handler handle_receive)
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			if (!proxy)
				handle_receive(error::service_stopped, address_type());
			else
				proxy->subscribe_address(handle_receive);
		}
		void channel::subscribe_get_address(
			channel_proxy::receive_get_address_handler handle_receive)
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			if (!proxy)
				handle_receive(error::service_stopped, get_address_type());
			else
				proxy->subscribe_get_address(handle_receive);
		}
		void channel::subscribe_inventory(
			channel_proxy::receive_inventory_handler handle_receive)
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			if (!proxy)
				handle_receive(error::service_stopped, inventory_type());
			else
				proxy->subscribe_inventory(handle_receive);
		}
		void channel::subscribe_get_data(
			channel_proxy::receive_get_data_handler handle_receive)
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			if (!proxy)
				handle_receive(error::service_stopped, get_data_type());
			else
				proxy->subscribe_get_data(handle_receive);
		}
		void channel::subscribe_get_blocks(
			channel_proxy::receive_get_blocks_handler handle_receive)
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			if (!proxy)
				handle_receive(error::service_stopped, get_blocks_type());
			else
				proxy->subscribe_get_blocks(handle_receive);
		}
		void channel::subscribe_transaction(
			channel_proxy::receive_transaction_handler handle_receive)
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			if (!proxy)
				handle_receive(error::service_stopped, transaction_type());
			else
				proxy->subscribe_transaction(handle_receive);
		}
		void channel::subscribe_block(
			channel_proxy::receive_block_handler handle_receive)
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			if (!proxy)
				handle_receive(error::service_stopped, block_type());
			else
				proxy->subscribe_block(handle_receive);
		}
		void channel::subscribe_raw(
			channel_proxy::receive_raw_handler handle_receive)
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			if (!proxy)
				handle_receive(error::service_stopped,
				header_type(), data_chunk());
			else
				proxy->subscribe_raw(handle_receive);
		}

		void channel::subscribe_stop(
			channel_proxy::stop_handler handle_stop)
		{
			channel_proxy_ptr proxy = weak_proxy_.lock();
			if (!proxy)
				handle_stop(error::service_stopped);
			else
				proxy->subscribe_stop(handle_stop);
		}

	}
}