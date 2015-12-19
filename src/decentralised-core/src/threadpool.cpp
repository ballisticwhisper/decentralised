#include <core/threadpool.hpp>

namespace decentralised
{
	namespace core
	{
		threadpool::threadpool()
			: work_(nullptr)
		{
		}

		threadpool::threadpool(size_t number_threads)
			: work_(nullptr)
		{
			spawn(number_threads);
		}

		threadpool::~threadpool()
		{
			delete work_;
		}

		void threadpool::spawn(size_t number_threads)
		{
			for (size_t i = 0; i < number_threads; ++i)
				spawn_once();
		}
		void threadpool::spawn_once()
		{
			if (!work_)
				work_ = new io_service::work(ios_);
			threads_.push_back(std::thread([this] { ios_.run(); }));
		}

		void threadpool::stop()
		{
			ios_.stop();
		}
		void threadpool::shutdown()
		{
			delete work_;
			work_ = nullptr;
		}
		void threadpool::join()
		{
			for (std::thread& t : threads_)
				t.join();
		}

		io_service& threadpool::service()
		{
			return ios_;
		}
		const io_service& threadpool::service() const
		{
			return ios_;
		}

		async_strand::async_strand(threadpool& pool)
			: ios_(pool.service()), strand_(ios_)
		{
		}

	}
}