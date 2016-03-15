#include <core/poller.hpp>

#include <core/utility/logger.hpp>

namespace decentralised
{
	namespace core
	{
		using std::placeholders::_1;
		using std::placeholders::_2;

		poller::poller(threadpool& pool, blockchain& chain)
			: strand_(pool.service()), chain_(chain),
			last_locator_begin_(null_hash), last_hash_stop_(null_hash),
			last_block_hash_(null_hash)
		{
		}

		void poller::query(channel_ptr node)
		{
			fetch_block_locator(chain_,
				std::bind(&poller::initial_ask_blocks,
				this, _1, _2, node));
		}

		void poller::monitor(channel_ptr node)
		{
			node->subscribe_inventory(
				strand_.wrap(std::bind(&poller::receive_inv,
				this, _1, _2, node)));
			node->subscribe_block(
				std::bind(&poller::receive_block,
				this, _1, _2, node));
		}

		void poller::initial_ask_blocks(const std::error_code& ec,
			const block_locator_type& locator, channel_ptr node)
		{
			if (ec)
			{
				log_error(LOG_POLLER)
					<< "Fetching initial block locator: " << ec.message();
				return;
			}
			strand_.dispatch(std::bind(&poller::ask_blocks,
				this, ec, locator, null_hash, node));
		}

		void handle_send_packet(const std::error_code& ec)
		{
			if (ec)
				log_error(LOG_POLLER) << "Send problem: " << ec.message();
		}

		void poller::receive_inv(const std::error_code& ec,
			const inventory_type& packet, channel_ptr node)
		{
			if (ec)
			{
				log_error(LOG_POLLER) << "Received bad inventory: " << ec.message();
				return;
			}
			// Filter out only block inventories
			get_data_type getdata;
			for (const inventory_vector_type& ivv : packet.inventories)
			{
				if (ivv.type != inventory_type_id::block)
					continue;
				// Already got this block
				if (ivv.hash == last_block_hash_)
					continue;
				getdata.inventories.push_back(ivv);
			}
			if (!getdata.inventories.empty())
			{
				last_block_hash_ = getdata.inventories.back().hash;
				node->send(getdata, handle_send_packet);
			}
			node->subscribe_inventory(
				strand_.wrap(std::bind(&poller::receive_inv,
				this, _1, _2, node)));
		}

		void poller::receive_block(const std::error_code& ec,
			const block_type& blk, channel_ptr node)
		{
			if (ec)
			{
				log_error(LOG_POLLER) << "Received bad block: " << ec.message();
				return;
			}
			chain_.store(blk,
				std::bind(&poller::handle_store,
				this, _1, _2, hash_block_header(blk.header), node));
			node->subscribe_block(
				std::bind(&poller::receive_block,
				this, _1, _2, node));
		}

		void poller::handle_store(const std::error_code& ec, block_info info,
			const hash_digest& block_hash, channel_ptr node)
		{
			// We need orphan blocks so we can do the next getblocks round
			if (ec && info.status != block_status::orphan)
			{
				log_error(LOG_POLLER)
					<< "Storing block " << encode_hex(block_hash)
					<< ": " << ec.message();
				return;
			}
			switch (info.status)
			{
			case block_status::orphan:
				// TODO: Make more efficient by storing block hash
				// and next time do not download orphan block again.
				// Remember to remove from list once block is no longer orphan
				fetch_block_locator(chain_,
					strand_.wrap(std::bind(&poller::ask_blocks,
					this, _1, _2, block_hash, node)));
				break;

			case block_status::rejected:
				log_error(LOG_POLLER)
					<< "Rejected block " << encode_hex(block_hash);
				break;

			case block_status::confirmed:
				log_info(LOG_POLLER)
					<< "Block #" << info.height << " " << encode_hex(block_hash);
				break;
			}
		}

		void poller::ask_blocks(const std::error_code& ec,
			const block_locator_type& locator,
			const hash_digest& hash_stop, channel_ptr node)
		{
			if (ec)
			{
				log_error(LOG_POLLER) << "Ask for blocks: " << ec.message();
				return;
			}
			if (last_locator_begin_ == locator.front() && last_hash_stop_ == hash_stop)
			{
				log_debug(LOG_POLLER) << "Skipping duplicate ask blocks: "
					<< encode_hex(locator.front());
				return;
			}
			get_blocks_type packet;
			packet.start_hashes = locator;
			packet.hash_stop = hash_stop;
			node->send(packet, std::bind(&handle_send_packet, _1));
			last_locator_begin_ = locator.front();
			last_hash_stop_ = hash_stop;
		}

	}
}