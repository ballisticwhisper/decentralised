#ifndef DC_CORE_ASYNC_PARALLEL_HPP
#define DC_CORE_ASYNC_PARALLEL_HPP

#include <atomic>
#include <memory>
#include <system_error>
#include <core/utility/assert.hpp>

namespace decentralised
{
	namespace core
	{
		/*
		 * async_parallel(completion_handler, clearance_count)
		 *
		 * Returns a callback that will stop when:
		 *
		 * 1. An error is passed as the first argument.
		 * 2. It has been called clearance_count number of times.
		 *
		 * This is useful when we have a piece of code that forks into several
		 * asynchronous code paths. This utility ensures that all code paths
		 * are completed before calling the completion_handler.
		 *
		 *   auto complete = [](std::error_code ec, std::string s) { ... };
		 *   auto cb = async_parallel(complete, 3);
		 *   cb(std::error_code, "hello");
		 *   // Calling cb(error::service_stopped, "") here will
		 *   // call complete(error::service_stopped, "")
		 *   cb(std::error_code, "world");
		 *   // Calls complete(std::error_code, "final")
		 *   cb(std::error_code, "final");
		 */

		typedef std::atomic<size_t> atomic_counter;
		typedef std::shared_ptr<atomic_counter> atomic_counter_ptr;

		template <typename Handler>
		struct async_parallel_dispatch
		{
			Handler handler;
			size_t clearance_count;
			atomic_counter_ptr counter;

			template <typename... Args>
			void operator()(const std::error_code& ec, Args&&... args)
			{
				BITCOIN_ASSERT(*counter <= clearance_count);
				if (*counter == clearance_count)
					return;
				if (ec)
				{
					// Stop because of failure.
					*counter = clearance_count;
					handler(ec, std::forward<Args>(args)...);
				}
				else if (++(*counter) == clearance_count)
					// Finished executing multiple async paths.
					handler(ec, std::forward<Args>(args)...);
			}
		};

		template <typename Handler>
		async_parallel_dispatch<
			typename std::decay<Handler>::type
		>
		async_parallel(Handler&& handler, size_t clearance_count)
		{
			return{ handler, clearance_count,
				std::make_shared<atomic_counter>(0) };
		}

	}
}
#endif

