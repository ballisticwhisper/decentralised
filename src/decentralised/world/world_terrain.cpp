//#include "world_terrain.h"
//
//namespace decentralised
//{
//	namespace world
//	{
//		world_terrain::world_terrain(IrrlichtDevice* device)
//		{
//			device_ = device;
//		}
//
//		world_terrain::~world_terrain()
//		{
//
//		}
//
//		void world_terrain::initialize()
//		{
//			ISceneManager *scene = device_->getSceneManager();
//			IVideoDriver *driver = device_->getVideoDriver();
//
//			driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
//
//			//char buffer[1024] = { 0 };
//			//io::IReadFile *memoryFile = io::createMemoryReadFile(&buffer, 1024, "", true);
//
//			sceneNode_ = scene->addTerrainSceneNode("data/assets/heightmap-test.bmp",
//													0, 
//													-1, 
//													vector3df(0, 0, 0), 
//													vector3df(0, 0, 0), 
//													vector3df(80.f, 4.4f, 80.f), 
//													irr::video::SColor(255, 255, 255, 255), 
//													5, 
//													ETPS_17,
//													4, 
//													true);
//			sceneNode_->setMaterialFlag(video::EMF_LIGHTING, false);
//
//			sceneNode_->setMaterialTexture(0,
//				driver->getTexture("data/assets/Default Set - Base Grass.png"));
//			sceneNode_->setMaterialTexture(1,
//				driver->getTexture("data/assets/Default Set - Grass.png"));
//
//			sceneNode_->setMaterialType(video::EMT_DETAIL_MAP);
//
//			sceneNode_->scaleTexture(1.0f, 20.0f);
//		}
//	}
//}