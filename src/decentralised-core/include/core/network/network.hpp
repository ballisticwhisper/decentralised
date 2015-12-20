#ifndef DC_CORE_NETWORK_NETWORK_HPP
#define DC_CORE_NETWORK_NETWORK_HPP

#include <boost/asio.hpp>
#include <boost/utility.hpp>
#include <memory>
#include <thread>
#include <core/primitives.hpp>
#include <core/types.hpp>
#include <core/error.hpp>
#include <core/threadpool.hpp>
#include <core/network/channel.hpp>

namespace decentralised
{
	namespace core
	{
		class acceptor
			: public std::enable_shared_from_this<acceptor>
		{
		public:
			typedef std::shared_ptr<tcp::acceptor> tcp_acceptor_ptr;

			typedef std::function<
				void(const std::error_code&, channel_ptr)> accept_handler;

			acceptor(threadpool& pool, tcp_acceptor_ptr tcp_accept);
			void accept(accept_handler handle_accept);

		private:
			void call_handle_accept(const boost::system::error_code& ec,
				socket_ptr socket, accept_handler handle_accept);

			threadpool& pool_;
			tcp_acceptor_ptr tcp_accept_;
		};

		class network
		{
		public:
			typedef std::function<
				void(const std::error_code&, channel_ptr)> connect_handler;
			typedef std::function<
				void(const std::error_code&, acceptor_ptr)> listen_handler;

			network(threadpool& pool);

			network(const network&) = delete;
			void operator=(const network&) = delete;

			void listen(uint16_t port, listen_handler handle_listen);
			void connect(const std::string& hostname, uint16_t port,
				connect_handler handle_connect);

		private:
			typedef std::shared_ptr<tcp::resolver> resolver_ptr;
			typedef std::shared_ptr<tcp::resolver::query> query_ptr;

			void resolve_handler(const boost::system::error_code& ec,
				tcp::resolver::iterator endpoint_iterator,
				connect_handler handle_connect, resolver_ptr, query_ptr);

			threadpool& pool_;
		};

	}
}
#endif

