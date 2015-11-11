#ifndef __MANAGER_NETWORK_H_INCLUDED__
#define __MANAGER_NETWORK_H_INCLUDED__

#include <irrlicht.h>
#include "dc-config.h"
#include "bitcoin/bitcoin.hpp"
#include <vector>
#include <map>

using namespace irr;

using namespace irr::core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace libbitcoin;

namespace decentralised
{
	namespace managers
	{
		class manager_network
		{
		public:
			manager_network();
			~manager_network();

			void initialize();

		private:
			threadpool disk_pool;
			threadpool net_pool;
		};
	}
}

#endif