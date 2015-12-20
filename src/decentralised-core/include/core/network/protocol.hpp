#ifndef DC_CORE_PROTOCOL_HPP
#define DC_CORE_PROTOCOL_HPP

#include <memory>
#include <system_error>
#include <core/types.hpp>
#include <core/primitives.hpp>
#include <core/network/channel.hpp>
#include <core/threadpool.hpp>
#include <core/utility/subscriber.hpp>
#include <core/utility/async_parallel.hpp>

namespace decentralised
{
	namespace core
	{
		class hosts;
		class handshake;

		class protocol
		{
		public:
			typedef std::function<void(const std::error_code&)> completion_handler;

			typedef std::function<void(const std::error_code&, size_t)>
				fetch_connection_count_handler;
			typedef std::function<void(const std::error_code&,
				channel_ptr)> channel_handler;

			typedef std::function<void(const std::error_code&, size_t)>
				broadcast_handler;

			protocol(threadpool& pool, hosts& hsts,
				handshake& shake, network& net);

			protocol(const protocol&) = delete;
			void operator=(const protocol&) = delete;

			/**
			 * Set max_outbound connections soft limit.
			 */
			void set_max_outbound(size_t max_outbound);

			/**
			 * Set the filename to load the hosts file from.
			 */
			void set_hosts_filename(const std::string& hosts_filename);

			/**
			 * If called, then this service will not listen for incoming
			 * connections. Must be called before start().
			 */
			void disable_listener();

			/**
			 * Perform full initialization sequence.
			 * Internally calls bootstrap() and then run().
			 *
			 * @param[in]   handle_complete     Completion handler for start operation.
			 * @code
			 *  void handle_complete(
			 *      const std::error_code& ec   // Status of operation
			 *  );
			 * @endcode
			 */
			void start(completion_handler handle_complete);

			/**
			 * Gracefully close down.
			 *
			 * @param[in]   handle_complete     Completion handler for start operation.
			 * @code
			 *  void handle_complete(
			 *      const std::error_code& ec   // Status of operation
			 *  );
			 * @endcode
			 */
			void stop(completion_handler handle_complete);

			/**
			 * Begin initialization sequence of performing node discovery and
			 * starting other network services.
			 *
			 * @param[in]   handle_complete     Completion handler for start operation.
			 * @code
			 *  void handle_complete(
			 *      const std::error_code& ec   // Status of operation
			 *  );
			 * @endcode
			 */
			void bootstrap(completion_handler handle_complete);

			/**
			 * Starts the internal run loop for this service.
			 */
			void run();

			/**
			 * Fetch number of connections maintained by this service.
			 *
			 * @param[in]   handle_fetch    Completion handler for fetch operation.
			 * @code
			 *  void handle_fetch(
			 *      const std::error_code& ec,  // Status of operation
			 *      size_t connection_count     // Number of connections
			 *  );
			 * @endcode
			 */
			void fetch_connection_count(
				fetch_connection_count_handler handle_fetch);

			/**
			 * Create a manual connection to a specific node. If disconnected
			 * this service will keep attempting to reconnect until successful.
			 *
			 * @param[in]   hostname            // Hostname
			 * @param[in]   port                // Port
			 */
			void maintain_connection(
				const std::string& hostname, uint16_t port);

			/**
			 * Subscribe to new connections established to other nodes.
			 * This method must be called again to stay subscribed as
			 * handlers are deregistered after being called.
			 *
			 * When this protocol service is stopped, any subscribed handlers
			 * will be called with the error_code set to error::service_stopped.
			 *
			 * @param[in]   handle_channel      Handler for new connection.
			 * @code
			 *  void handle_channel(
			 *      const std::error_code& ec,  // Status of operation
			 *      channel_ptr node            // Communication channel to new node
			 *  );
			 * @endcode
			 */
			void subscribe_channel(channel_handler handle_channel);

			/**
			 * Broadcast a message to all nodes in our connection list.
			 *
			 * @param[in]   packet      Message packet to broadcast
			 * @param[in]   handle_send Called after every send operation.
			 * @code
			 *  void handle_send(
			 *      const std::error_code& ec,  // Status of operation
			 *      size_t total_nodes          // Total number of connected nodes
			 *                                  // when the operation began.
			 *  );
			 * @endcode
			 */
			template <typename Message>
			void broadcast(const Message& packet, broadcast_handler handle_send)
			{
				// The intermediate variable 'lambda' is a workaround for a
				// limitation of the VC++ CTP_Nov2013 generic lambda support.
				auto lambda = &protocol::do_broadcast<Message>;
				strand_.queue(lambda, this, packet, handle_send);
			}

		private:
			struct connection_info
			{
				network_address_type address;
				channel_ptr node;
			};
			typedef std::vector<connection_info> connection_list;
			enum class connect_state
			{
				finding_peer,
				connecting,
				established,
				stopped
			};
			typedef std::vector<connect_state> connect_state_list;
			typedef size_t slot_index;
			// Accepted connections
			typedef std::vector<channel_ptr> channel_ptr_list;

			typedef subscriber<const std::error_code&, channel_ptr>
				channel_subscriber_type;

			// start sequence
			void handle_bootstrap(
				const std::error_code& ec, completion_handler handle_complete);

			// stop sequence
			void handle_save(const std::error_code& ec,
				completion_handler handle_complete);

			// bootstrap sequence
			void load_hosts(const std::error_code& ec,
				completion_handler handle_complete);
			void if_0_seed(const std::error_code& ec, size_t hosts_count,
				completion_handler handle_complete);

			// seed addresses from dns seeds
			class seeds
				: public std::enable_shared_from_this<seeds>
			{
			public:
				seeds(protocol* parent);
				void start(completion_handler handle_complete);
			private:
				void error_case(const std::error_code& ec);

				void connect_dns_seed(const std::string& hostname);
				void request_addresses(const std::error_code& ec,
					channel_ptr dns_seed_node);
				void handle_send_get_address(const std::error_code& ec);

				void save_addresses(const std::error_code& ec,
					const address_type& packet, channel_ptr);
				void handle_store(const std::error_code& ec);

				completion_handler handle_complete_;
				size_t ended_paths_;
				bool finished_;

				// From parent
				async_strand& strand_;
				hosts& hosts_;
				handshake& handshake_;
				network& network_;
			};
			std::shared_ptr<seeds> load_seeds_;
			friend class seeds;

			std::string state_to_string(connect_state state);
			void modify_slot(slot_index slot, connect_state state);

			// run loop
			void start_connecting();
			// Connect outwards
			void start_stopped_connects();
			// This function is called in these places:
			//
			// 1. try_outbound_connects() calls it n times.
			// Called by run() at the start.
			// 2. If we fetch a random node address that we are already
			// connected to in attempt_connect().
			// 3. If we fail to connect to said address in handle_connect().
			// 4. If the channel is stopped manually or there is an error
			// (such as a disconnect). See setup_new_channel() for the
			// subscribe call.
			void try_connect_once(slot_index slot);
			void attempt_connect(const std::error_code& ec,
				const network_address_type& packet, slot_index slot);
			void handle_connect(const std::error_code& ec, channel_ptr node,
				const network_address_type& address, slot_index slot);

			// Periodically call this method to reset the watermark and reallow
			// connections. This prevents too many connection attempts from
			// exhausting resources by putting a limit on connection attempts
			// within a certain time interval.
			void start_watermark_reset_timer();

			// Manual connections
			void handle_manual_connect(const std::error_code& ec, channel_ptr node,
				const std::string& hostname, uint16_t port);

			// Accept inwards connections
			void handle_listen(const std::error_code& ec, acceptor_ptr accept);
			void handle_accept(const std::error_code& ec, channel_ptr node,
				acceptor_ptr accept);

			// Channel setup
			void setup_new_channel(channel_ptr node);

			// Remove channels from lists when disconnected.
			void outbound_channel_stopped(
				const std::error_code& ec, channel_ptr which_node, slot_index slot);
			void manual_channel_stopped(
				const std::error_code& ec, channel_ptr which_node,
				const std::string& hostname, uint16_t port);
			void inbound_channel_stopped(
				const std::error_code& ec, channel_ptr which_node);

			void subscribe_address(channel_ptr node);
			void receive_address_message(const std::error_code& ec,
				const address_type& addr, channel_ptr node);
			void handle_store_address(const std::error_code& ec);

			// fetch methods
			void do_fetch_connection_count(
				fetch_connection_count_handler handle_fetch);

			template <typename Message>
			void do_broadcast(const Message& packet, broadcast_handler handle_send)
			{
				size_t total_nodes = connections_.size() + accepted_channels_.size();
				auto send_handler =
					std::bind(handle_send, std::placeholders::_1, total_nodes);
				for (const connection_info& connection : connections_)
					connection.node->send(packet, send_handler);
				for (channel_ptr node : manual_connections_)
					node->send(packet, send_handler);
				for (channel_ptr node : accepted_channels_)
					node->send(packet, send_handler);
			}

			async_strand strand_;

			std::string hosts_filename_ = "hosts.p2p";
			hosts& hosts_;
			handshake& handshake_;
			network& network_;

			// There's a fixed number of slots that are always trying to reconnect.
			size_t max_outbound_ = 8;
			connection_list connections_;
			// Simply a debugging tool to enforce correct state transition behaviour
			// for maintaining connections.
			connect_state_list connect_states_;
			// Used to prevent too many connection attempts from exhausting resources.
			// The watermark is refreshed every interval.
			boost::asio::deadline_timer watermark_timer_;
			size_t watermark_count_ = 0;

			// Manual connections created by user themselves.
			channel_ptr_list manual_connections_;

			// Inbound connections from the p2p network.
			bool listen_is_enabled_ = true;
			channel_ptr_list accepted_channels_;

			channel_subscriber_type::ptr channel_subscribe_;
		};

	}
}
#endif

