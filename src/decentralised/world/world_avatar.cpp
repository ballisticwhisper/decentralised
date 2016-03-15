#include "world_avatar.h"

namespace decentralised
{
	namespace world
	{
		world_avatar::world_avatar(IrrlichtDevice* device)
		{
			device_ = device;
		}

		world_avatar::~world_avatar()
		{

		}

		void world_avatar::initialize()
		{
			core::vector3df targetPos(3000, 0, 3000);

			ISceneManager *sceneManager = device_->getSceneManager();
			IVideoDriver *driver = device_->getVideoDriver();

			camera_ = sceneManager->addCameraSceneNode(0);

			camera_->setPosition(core::vector3df(2700 * 2, 200, 2600 * 2));
			camera_->setTarget(targetPos);
			camera_->setFarValue(42000.0f);

			mesh_ = device_->getSceneManager()->getMesh("data/assets/Skeleton.Male.x");
			meshNode_ = device_->getSceneManager()->addAnimatedMeshSceneNode(mesh_);
			meshNode_->setAnimationSpeed(30);
			meshNode_->setMaterialFlag(video::EMF_LIGHTING, false);
			meshNode_->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, false);
			meshNode_->setDebugDataVisible(scene::EDS_OFF);
			meshNode_->setScale(core::vector3df(200,200,200));
			meshNode_->setPosition(targetPos);

			//core::quaternion q;
			//q.fromAngleAxis(90 * core::DEGTORAD, core::vector3df(1, 0, 0));
			//core::matrix4 m1 = q.getMatrix();

			//meshNode_->updateAbsolutePosition();
			//core::matrix4 m2 = meshNode_->getAbsoluteTransformation();

			//core::matrix4 m = m1*m2;
			//meshNode_->setRotation(m.getRotationDegrees());

			ITexture* test = driver->getTexture("data/assets/upper-body.png");
			meshNode_->getMaterial(0).TextureLayer[0].Texture = test;
			ITexture* test2 = driver->getTexture("data/assets/lower-body.png");
			meshNode_->getMaterial(1).TextureLayer[0].Texture = test2;
			ITexture* test3 = driver->getTexture("data/assets/head.png");
			meshNode_->getMaterial(2).TextureLayer[0].Texture = test3;

			//meshNode_->setJointMode(EJUOR_CONTROL);
			IBoneSceneNode* rightUpperArm = meshNode_->getJointNode((u32)0);
			core::vector3df cpos = rightUpperArm->getRotation();
			cpos.Z -= 1.5;
			rightUpperArm->setRotation(cpos);
		}

		ICameraSceneNode* world_avatar::getCamera()
		{
			return camera_;
		}
	}
}