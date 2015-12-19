#ifndef  DC_CORE_UTILITY_SUBSCRIBER_HPP
#define  DC_CORE_UTILITY_SUBSCRIBER_HPP

#include <stack>

#include <core/types.hpp>
#include <core/threadpool.hpp>
#include <core/utility/assert.hpp>

namespace decentralised
{
	namespace core
	{
		template <typename... Args>
		class subscriber
			: public std::enable_shared_from_this<subscriber<Args...>>
		{
		public:
			typedef std::function<void(Args...)> handler_type;
			typedef std::shared_ptr<subscriber<Args...>> ptr;

			subscriber(threadpool& pool)
				: strand_(pool.service())
			{
			}

			void subscribe(handler_type handle)
			{
				strand_.dispatch(
					std::bind(&subscriber<Args...>::do_subscribe,
					this->shared_from_this(), handle));
			}

			void relay(Args... params)
			{
				strand_.dispatch(
					std::bind(&subscriber<Args...>::do_relay,
					this->shared_from_this(), std::forward<Args>(params)...));
			}

		private:
			typedef std::stack<handler_type> registry_stack;

			void do_subscribe(handler_type handle)
			{
				registry_.push(handle);
			}

			void do_relay(Args... params)
			{
				registry_stack notify_copy = registry_;
				registry_ = registry_stack();
				while (!notify_copy.empty())
				{
					notify_copy.top()(params...);
					notify_copy.pop();
				}
				BITCOIN_ASSERT(notify_copy.empty());
			}

			io_service::strand strand_;
			registry_stack registry_;
		};

		template <typename... Args>
		using subscriber_ptr = std::shared_ptr<subscriber<Args...>>;

	}
}
#endif

