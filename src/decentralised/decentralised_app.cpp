#include "decentralised_app.h"

namespace decentralised
{
	decentralised_app::decentralised_app()
	{
	}

	decentralised_app::~decentralised_app()
	{
		Ogre::WindowEventUtilities::removeWindowEventListener(window_, this);
		windowClosed(window_);

		delete context_.ogre_root;
	}

	void decentralised_app::go()
	{
		manager_filesystem* fileManager = new manager_filesystem();

		context_.ogre_root = new Ogre::Root("", "", "decentralised.log");

		Ogre::D3D9RenderSystem* d3d9 = new Ogre::D3D9RenderSystem(GetModuleHandle(NULL));
		Ogre::Root::getSingleton().addRenderSystem(d3d9);

		//Ogre::D3D11RenderSystem* d3d11 = new Ogre::D3D11RenderSystem();
		//Ogre::Root::getSingleton().addRenderSystem(d3d11);

		Ogre::GLRenderSystem* opengl = new Ogre::GLRenderSystem();
		Ogre::Root::getSingleton().addRenderSystem(opengl);

		Ogre::RenderSystemList devices = context_.ogre_root->getAvailableRenderers();
		for (auto it = devices.begin(); it != devices.end(); ++it) {
			Ogre::LogManager::getSingleton().logMessage((*it)->getName());
		}

		// TODO: Choose either saved setting, d3d11, d3d9, or OpenGL in that order.
		context_.ogre_root->setRenderSystem(d3d9);
		d3d9->setConfigOption("Full Screen", "No");
		d3d9->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

		context_.config = fileManager->loadConfig();
		context_.skin = fileManager->loadSkin(context_.config[L"Skin"]);
		context_.language = fileManager->loadLanguage(context_.config[L"Language"]);

		std::wstring title = std::wstring(APP_TITLE)
			.append(L" ")
			.append(to_wstring(Decentralised_VERSION_MAJOR))
			.append(L".")
			.append(to_wstring(Decentralised_VERSION_MINOR))
			.append(L"\n").c_str();

		// Create window
		window_ = context_.ogre_root->initialise(true, std::string(title.begin(), title.end()));

		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

		OIS::ParamList pl;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;

		window_->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));



		Ogre::WindowEventUtilities::addWindowEventListener(window_, this);
		context_.ogre_root->addFrameListener(this);

		Ogre::LogManager::getSingletonPtr()->logMessage("Finished initializing OIS");

		//context.gui_manager = new manager_gui(context.ogre_root, context.language, context.skin, context.config);
		//context.gui_manager->Initialize();

		//context.world_manager = new manager_world(context.ogre_root);

		//IVideoDriver* driver = context.device->getVideoDriver();

		//context.gui_manager->AddConsoleLine(SColor(255,255,255,255), APP_TITLE);

		//irrlicht_event_handler receiver(context);

		//context.network_manager = new manager_core();

		//context.network_manager->setEventReceiver(&receiver);
		//context.device->setEventReceiver(&receiver);

		//context.network_manager->start();

		//ISceneManager *scene = context.device->getSceneManager();

		//context_.ogre_root->startRendering();

		while (!window_->isClosed())
		{
			Ogre::WindowEventUtilities::messagePump();
			context_.ogre_root->renderOneFrame();
		//	//	if (!context.device->isWindowMinimized() || context.device->isWindowActive())
		//	//	{
		//	//		driver->beginScene(true, true, SColor(0, 0, 0, 0));

		//	//		scene->drawAll();
		//	//		context.gui_manager->DrawAll();

		//	//		driver->endScene();
		}

		//context.network_manager->stop();

		//delete context.world_manager;
		//delete context.gui_manager;
		//delete context.network_manager;
		//delete fileManager;
	}

	bool decentralised_app::keyPressed(const OIS::KeyEvent& ke)
	{
		return true;
	}

	bool decentralised_app::keyReleased(const OIS::KeyEvent& ke)
	{
		return true;
	}

	bool decentralised_app::mouseMoved(const OIS::MouseEvent& me)
	{
		return true;
	}

	bool decentralised_app::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id)
	{
		return true;
	}

	bool decentralised_app::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id)
	{
		return true;
	}

	void decentralised_app::windowResized(Ogre::RenderWindow* rw)
	{

	}

	void decentralised_app::windowClosed(Ogre::RenderWindow* rw)
	{

	}
}