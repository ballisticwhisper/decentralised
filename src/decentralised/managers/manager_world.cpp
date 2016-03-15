#include "manager_world.h"

namespace decentralised
{
	namespace managers
	{
		manager_world::manager_world(IrrlichtDevice* device) :
			terrain(device), atmosphere(device), avatar(device)
		{
			device_ = device;
		}

		manager_world::~manager_world()
		{

		}

		void manager_world::initialize()
		{
			terrain.initialize();
			atmosphere.initialize();
			avatar.initialize();
		}

		world_avatar* manager_world::getAvatar()
		{
			return &avatar;
		}
	}
}