#include "world_atmosphere.h"

namespace decentralised
{
	namespace world
	{
		world_atmosphere::world_atmosphere(IrrlichtDevice* device)
		{
			device_ = device;
		}

		world_atmosphere::~world_atmosphere()
		{

		}

		void world_atmosphere::initialize()
		{
			ISceneManager *scene = device_->getSceneManager();
			IVideoDriver *driver = device_->getVideoDriver();

			driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

			skyBox_ = scene->addSkyBoxSceneNode(
				driver->getTexture("data/assets/sky-top.jpg"),
				driver->getTexture("data/assets/sky-bottom.jpg"),
				driver->getTexture("data/assets/sky-sides.jpg"),
				driver->getTexture("data/assets/sky-sides.jpg"),
				driver->getTexture("data/assets/sky-sides.jpg"),
				driver->getTexture("data/assets/sky-sides.jpg")
				);

			driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
		}
	}
}