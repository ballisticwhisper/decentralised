#ifndef DC_CORE_SATOSHI_SERIALIZE_HPP
#define DC_CORE_SATOSHI_SERIALIZE_HPP

#include <core/constants.hpp>
#include <core/format.hpp>
#include <core/primitives.hpp>
#include <core/transaction.hpp>
#include <core/utility/assert.hpp>
#include <core/utility/logger.hpp>
#include <core/utility/serializer.hpp>

namespace decentralised
{
	namespace core
	{
		constexpr size_t command_size = 12;

		// message headers
		size_t satoshi_raw_size(const header_type& head);
		template <typename Iterator>
		Iterator satoshi_save(const header_type& head, Iterator result);
		template <typename Iterator>
		void satoshi_load(const Iterator first, const Iterator last,
			header_type& head);

		// version messages
		const std::string satoshi_command(const version_type&);
		size_t satoshi_raw_size(const version_type& packet);
		template <typename Iterator>
		Iterator satoshi_save(const version_type& packet, Iterator result);
		template <typename Iterator>
		void satoshi_load(const Iterator first, const Iterator last,
			version_type& packet);

		// verack messages
		const std::string satoshi_command(const verack_type&);
		size_t satoshi_raw_size(const verack_type& packet);
		template <typename Iterator>
		Iterator satoshi_save(const verack_type& packet, Iterator result);
		template <typename Iterator>
		void satoshi_load(const Iterator first, const Iterator last,
			verack_type& packet);

		// addr messages
		const std::string satoshi_command(const address_type&);
		size_t satoshi_raw_size(const address_type& packet);
		template <typename Iterator>
		Iterator satoshi_save(const address_type& packet, Iterator result);
		template <typename Iterator>
		void satoshi_load(const Iterator first, const Iterator last,
			address_type& packet);

		// getaddr messages
		const std::string satoshi_command(const get_address_type&);
		size_t satoshi_raw_size(const get_address_type& packet);
		template <typename Iterator>
		Iterator satoshi_save(const get_address_type& packet, Iterator result);
		template <typename Iterator>
		void satoshi_load(const Iterator first, const Iterator last,
			get_address_type& packet);

		// inv messages
		const std::string satoshi_command(const inventory_type&);
		size_t satoshi_raw_size(const inventory_type& packet);
		template <typename Iterator>
		Iterator satoshi_save(const inventory_type& packet, Iterator result);
		template <typename Iterator>
		void satoshi_load(const Iterator first, const Iterator last,
			inventory_type& packet);

		// getdata messages
		const std::string satoshi_command(const get_data_type&);
		size_t satoshi_raw_size(const get_data_type& packet);
		template <typename Iterator>
		Iterator satoshi_save(const get_data_type& packet, Iterator result);
		template <typename Iterator>
		void satoshi_load(const Iterator first, const Iterator last,
			get_data_type& packet);

		// getblocks messages
		const std::string satoshi_command(const get_blocks_type&);
		size_t satoshi_raw_size(const get_blocks_type& packet);
		template <typename Iterator>
		Iterator satoshi_save(const get_blocks_type& packet, Iterator result);
		template <typename Iterator>
		void satoshi_load(const Iterator first, const Iterator last,
			get_blocks_type& packet);

		// tx messages
		const std::string satoshi_command(const transaction_type&);
		size_t satoshi_raw_size(const transaction_type& packet);
		template <typename Iterator>
		Iterator satoshi_save(const transaction_type& packet, Iterator result);
		template <typename Iterator>
		void satoshi_load(const Iterator first, const Iterator last,
			transaction_type& packet);

		// block header
		size_t satoshi_raw_size(const block_header_type& packet);
		template <typename Iterator>
		Iterator satoshi_save(const block_header_type& packet, Iterator result);
		template <typename Iterator>
		void satoshi_load(const Iterator first, const Iterator last,
			block_header_type& packet);

		// block messages
		const std::string satoshi_command(const block_type&);
		size_t satoshi_raw_size(const block_type& packet);
		template <typename Iterator>
		Iterator satoshi_save(const block_type& packet, Iterator result);
		template <typename Iterator>
		void satoshi_load(const Iterator first, const Iterator last,
			block_type& packet);

		// ping messages
		const std::string satoshi_command(const ping_type&);
		size_t satoshi_raw_size(const ping_type& packet);
		template <typename Iterator>
		Iterator satoshi_save(const ping_type& packet, Iterator result);
		template <typename Iterator>
		void satoshi_load(const Iterator first, const Iterator last,
			ping_type& packet);

		// pong messages
		const std::string satoshi_command(const pong_type&);
		size_t satoshi_raw_size(const pong_type& packet);
		template <typename Iterator>
		Iterator satoshi_save(const pong_type& packet, Iterator result);
		template <typename Iterator>
		void satoshi_load(const Iterator first, const Iterator last,
			pong_type& packet);

	}
}
#include <core/impl/serialize/misc.ipp>
#include <core/impl/serialize/block.ipp>

#endif

