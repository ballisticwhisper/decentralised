//#ifndef __MANAGER_WORLD_H_INCLUDED__
//#define __MANAGER_WORLD_H_INCLUDED__
//
//#include <irrlicht.h>
//#include "dc-config.h"
//#include "world_terrain.h"
//#include "world_atmosphere.h"
//#include "world_avatar.h"
//
//using namespace irr;
//
//using namespace irr::core;
//using namespace scene;
//using namespace video;
//using namespace io;
//using namespace gui;
//
//using namespace decentralised;
//using namespace decentralised::world;
//
//namespace decentralised
//{
//	namespace managers
//	{
//		class manager_world
//		{
//		public:
//			manager_world(IrrlichtDevice* device);
//			~manager_world();
//
//			void initialize();
//
//			world_avatar* manager_world::getAvatar();
//
//		private:
//			world_terrain terrain;
//			world_atmosphere atmosphere;
//			world_avatar avatar;
//
//			IrrlichtDevice* device_;
//			scene::ICameraSceneNode* camera_;
//		};
//	}
//}
//
//#endif