#ifndef DC_CORE_BLOCKCHAIN_BLOCKCHAIN_HPP
#define DC_CORE_BLOCKCHAIN_BLOCKCHAIN_HPP

/**
 * Represents an interface to a blockchain backend.
 */

#include <boost/utility.hpp>
#include <functional>
#include <core/block.hpp>
#include <core/error.hpp>
#include <core/primitives.hpp>
#include <core/address.hpp>
#include <core/transaction.hpp>
#include <core/stealth.hpp>

namespace decentralised
{
	namespace core
	{
		class blockchain
		{
		public:
			typedef std::function<void(const std::error_code&, block_info)>
				store_block_handler;

			typedef std::function<void(const std::error_code&)> import_block_handler;

			template <typename Message>
			using fetch_handler = std::function<
				void(const std::error_code&, const Message&)>;

			typedef fetch_handler<block_header_type> fetch_handler_block_header;

			typedef fetch_handler<hash_digest_list>
				fetch_handler_block_transaction_hashes;

			typedef fetch_handler<size_t> fetch_handler_block_height;

			typedef fetch_handler<size_t> fetch_handler_last_height;

			typedef fetch_handler<block_locator_type>
				fetch_handler_block_locator;

			typedef fetch_handler<transaction_type> fetch_handler_transaction;

			typedef std::function<
				void(const std::error_code&, size_t, size_t)>
				fetch_handler_transaction_index;

			typedef fetch_handler<input_point> fetch_handler_spend;

			struct history_row
			{
				output_point output;
				size_t output_height;
				uint64_t value;
				input_point spend;
				size_t spend_height;
			};
			typedef std::vector<history_row> history_list;
			typedef std::function<void(const std::error_code&, const history_list&)>
				fetch_handler_history;

			struct stealth_row
			{
				data_chunk ephemkey;
				payment_address address;
				hash_digest transaction_hash;
			};
			typedef std::vector<stealth_row> stealth_list;
			typedef std::function<void(const std::error_code&, const stealth_list&)>
				fetch_handler_stealth;

			typedef std::vector<std::shared_ptr<block_type>> block_list;
			typedef std::function<
				void(const std::error_code&, size_t,
				const block_list&, const block_list&)> reorganize_handler;

			virtual ~blockchain() {};

			/**
			 * Store a new block.
			 *
			 * Subscriber is notified exactly once of changes to the blockchain
			 * and needs to re-subscribe to continue being notified.
			 *
			 * @param[in]   block           Block to store
			 * @param[in]   handle_store    Completion handler for store operation.
			 * @code
			 *  void handle_store(
			 *      const std::error_code& ec,   // Status of operation
			 *      block_info info              // Status and height of block
			 *  );
			 * @endcode
			 */
			virtual void store(const block_type& block,
				store_block_handler handle_store) = 0;

			/**
			 * Store a new block directly without validating it.
			 * No checks are done. Importing an already stored block
			 * is undefined.
			 *
			 * @param[in]   import_block    Block to store
			 * @param[in]   height          Height of block
			 * @param[in]   handle_import   Completion handler for import operation.
			 * @code
			 *  void handle_import(
			 *      const std::error_code& ec   // Status of operation
			 *  );
			 * @encode
			 */
			virtual void import(const block_type& import_block, size_t height,
				import_block_handler handle_import) = 0;

			/**
			 * Fetches the block header by height.
			 *
			 * @param[in]   height          Height of block to fetch
			 * @param[in]   handle_fetch    Completion handler for fetch operation.
			 * @code
			 *  void handle_fetch(
			 *      const std::error_code& ec,      // Status of operation
			 *      const block_header_type& blk    // Block header
			 *  );
			 * @endcode
			 */
			virtual void fetch_block_header(size_t height,
				fetch_handler_block_header handle_fetch) = 0;

			/**
			 * Fetches the block header by hash.
			 *
			 * @param[in]   block_hash      Hash of block
			 * @param[in]   handle_fetch    Completion handler for fetch operation.
			 * @code
			 *  void handle_fetch(
			 *      const std::error_code& ec,      // Status of operation
			 *      const block_header_type& blk    // Block header
			 *  );
			 * @endcode
			 */
			virtual void fetch_block_header(const hash_digest& block_hash,
				fetch_handler_block_header handle_fetch) = 0;

			/**
			 * Fetches list of transaction hashes in a block by height.
			 *
			 * @param[in]   height          Height of block containing transactions.
			 * @param[in]   handle_fetch    Completion handler for fetch operation.
			 * @code
			 *  void handle_fetch(
			 *      const std::error_code& ec,      // Status of operation
			 *      const hash_digest_list& hashes  // List of hashes
			 *  );
			 * @endcode
			 */
			virtual void fetch_block_transaction_hashes(size_t height,
				fetch_handler_block_transaction_hashes handle_fetch) = 0;

			/**
			 * Fetches list of transaction hashes in a block by block hash.
			 *
			 * @param[in]   block_hash      Hash of block
			 * @param[in]   handle_fetch    Completion handler for fetch operation.
			 * @code
			 *  void handle_fetch(
			 *      const std::error_code& ec,      // Status of operation
			 *      const hash_digest_list& hashes  // List of hashes
			 *  );
			 * @endcode
			 */
			virtual void fetch_block_transaction_hashes(
				const hash_digest& block_hash,
				fetch_handler_block_transaction_hashes handle_fetch) = 0;

			/**
			 * Fetches the height of a block given its hash.
			 *
			 * @param[in]   block_hash      Hash of block
			 * @param[in]   handle_fetch    Completion handler for fetch operation.
			 * @code
			 *  void handle_fetch(
			 *      const std::error_code& ec, // Status of operation
			 *      size_t block_height        // Height of block
			 *  );
			 * @endcode
			 */
			virtual void fetch_block_height(const hash_digest& block_hash,
				fetch_handler_block_height handle_fetch) = 0;

			/**
			 * Fetches the height of the last block in our blockchain.
			 *
			 * @param[in]   handle_fetch    Completion handler for fetch operation.
			 * @code
			 *  void handle_fetch(
			 *      const std::error_code& ec, // Status of operation
			 *      size_t block_height        // Height of last block
			 *  );
			 * @endcode
			 */
			virtual void fetch_last_height(
				fetch_handler_last_height handle_fetch) = 0;

			/**
			 * Fetches a transaction by hash.
			 *
			 * @param[in]   transaction_hash  Transaction's hash
			 * @param[in]   handle_fetch      Completion handler for fetch operation.
			 * @code
			 *  void handle_fetch(
			 *      const std::error_code& ec,  // Status of operation
			 *      const transaction_type& tx  // Transaction
			 *  );
			 * @endcode
			 */
			virtual void fetch_transaction(const hash_digest& transaction_hash,
				fetch_handler_transaction handle_fetch) = 0;

			/**
			 * Fetch the block height that contains a transaction and its index
			 * within a block.
			 *
			 * @param[in]   transaction_hash  Transaction's hash
			 * @param[in]   handle_fetch      Completion handler for fetch operation.
			 * @code
			 *  void handle_fetch(
			 *      const std::error_code& ec, // Status of operation
			 *      size_t block_height,       // Height of block containing
			 *                                 // the transaction.
			 *      size_t index               // Index of transaction within
			 *                                 // the block.
			 *  );
			 * @endcode
			 */
			virtual void fetch_transaction_index(
				const hash_digest& transaction_hash,
				fetch_handler_transaction_index handle_fetch) = 0;

			// fetch of inputs and outputs is a future possibility
			// for now use fetch_transaction and lookup input/output

			/**
			 * Fetches a corresponding spend of an output.
			 *
			 * @param[in]   outpoint        Representation of an output.
			 * @param[in]   handle_fetch    Completion handler for fetch operation.
			 * @code
			 *  void handle_fetch(
			 *      const std::error_code& ec,      // Status of operation
			 *      const input_point& inpoint      // Spend of output
			 *  );
			 * @endcode
			 */
			virtual void fetch_spend(const output_point& outpoint,
				fetch_handler_spend handle_fetch) = 0;

			/**
			 * Fetches the output points, output values, corresponding input point
			 * spends and the block heights associated with a Bitcoin address.
			 * The returned history is a list of rows with the following fields:
			 *
			 * @code
			 *  struct history_row
			 *  {
			 *      output_point output;
			 *      size_t output_height;
			 *      uint64_t value;
			 *      input_point spend;
			 *      size_t spend_height;
			 *  };
			 * @endcode
			 *
			 * If an output is unspent then the input spend hash will be equivalent
			 * to null_hash.
			 *
			 * @code
			 *  if (history.spend.hash == null_hash)
			 *    // The history.output point is unspent.
			 * @endcode
			 *
			 * Summing the list of values for unspent outpoints gives the balance
			 * for an address.
			 *
			 * @param[in]   address         Bitcoin address
			 * @param[in]   handle_fetch    Completion handler for fetch operation.
			 * @param[in]   from_height     Starting block height for history.
			 *                              Useful to filter entries or to fetch
			 *                              the history in chunks.
			 * @code
			 *  void handle_fetch(
			 *      const std::error_code& ec,              // Status of operation
			 *      const blockchain::history_list& history // History
			 *  );
			 * @endcode
			 */
			virtual void fetch_history(const payment_address& address,
				fetch_handler_history handle_fetch, size_t from_height = 0) = 0;

			/**
			 * Fetch possible stealth results. These results can then be iterated
			 * to discover new payments belonging to a particular stealth address.
			 * This is for recipient privacy.
			 * The result is a list of rows with the following fields:
			 *
			 * @code
			 *  struct stealth_row
			 *  {
			 *      data_chunk ephemkey;
			 *      payment_address address;
			 *      hash_digest transaction_hash;
			 *  };
			 * @endcode
			 *
			 * The prefix is a special value that can be adjusted to provide
			 * greater precision at the expense of deniability.
			 *
			 * from_height is not guaranteed to only return results from that
			 * height, and may also include results from earlier blocks.
			 * It is provided as an optimisation. All results at and after
			 * from_height are guaranteed to be returned however.
			 *
			 * @param[in]   prefix          Stealth prefix information.
			 * @param[in]   handle_fetch    Completion handler for fetch operation.
			 * @param[in]   from_height     Starting block height for stealth results.
			 *
			 * @code
			 *  void handle_fetch(
			 *      const std::error_code& ec,                   // Status of operation
			 *      const blockchain::stealth_list& stealth_rows // Stealth result rows
			 *  );
			 * @endcode
			 */
			virtual void fetch_stealth(const stealth_prefix& prefix,
				fetch_handler_stealth handle_fetch, size_t from_height = 0) = 0;

			/**
			 * Be notified of the next blockchain change.
			 *
			 * Subscriber is notified exactly once of changes to the blockchain
			 * and needs to re-subscribe to continue being notified.
			 *
			 * @param[in]   handle_reorganize   Notification handler for changes
			 * @code
			 *  void handle_reorganize(
			 *      const std::error_code& ec,   // Status of operation
			 *      size_t fork_point,           // Index where blockchain forks
			 *      const block_list& added,     // New blocks added to blockchain
			 *      const block_list& removed    // Blocks removed (empty if none)
			 *  );
			 * @endcode
			 */
			virtual void subscribe_reorganize(
				reorganize_handler handle_reorganize) = 0;
		};

		typedef std::function<void(const std::error_code&, const block_type&)>
			blockchain_fetch_handler_block;

		/**
		 * Fetch a block by height.
		 *
		 * If the blockchain reorganises, operation may fail halfway.
		 *
		 * @param[in]   chain           Blockchain service
		 * @param[in]   height          Height of block to fetch.
		 * @param[in]   handle_fetch    Completion handler for fetch operation.
		 * @code
		 *  void handle_fetch(
		 *      const std::error_code& ec,  // Status of operation
		 *      const block_type& blk       // Block header
		 *  );
		 * @endcode
		 */
		void fetch_block(blockchain& chain, size_t height,
			blockchain_fetch_handler_block handle_fetch);

		/**
		 * Fetch a block by hash.
		 *
		 * If the blockchain reorganises, operation may fail halfway.
		 *
		 * @param[in]   chain           Blockchain service
		 * @param[in]   block_hash      Hash of block to fetch.
		 * @param[in]   handle_fetch    Completion handler for fetch operation.
		 * @code
		 *  void handle_fetch(
		 *      const std::error_code& ec,  // Status of operation
		 *      const block_type& blk       // Block header
		 *  );
		 * @endcode
		 */
		void fetch_block(blockchain& chain, const hash_digest& block_hash,
			blockchain_fetch_handler_block handle_fetch);

		typedef std::function<
			void(const std::error_code&, const block_locator_type&)>
			blockchain_fetch_handler_block_locator;

		/**
		 * Creates a block_locator object used to download the blockchain.
		 *
		 * @param[in]   handle_fetch    Completion handler for fetch operation.
		 * @code
		 *  void handle_fetch(
		 *      const std::error_code& ec,      // Status of operation
		 *      const block_locator_type& loc   // Block locator object
		 *  );
		 * @endcode
		 */
		void fetch_block_locator(blockchain& chain,
			blockchain_fetch_handler_block_locator handle_fetch);

		typedef std::function<void(const std::error_code&,
			const output_point_list&, const input_point_list&)>
			blockchain_fetch_handler_history;

	}
}
#endif

