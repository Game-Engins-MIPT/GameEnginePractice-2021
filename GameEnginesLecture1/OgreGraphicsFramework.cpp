#include "OgreGraphicsFramework.h"

OgreGraphicsFramework::OgreGraphicsFramework() :
	m_pRoot(nullptr),
	m_pRenderWindow(nullptr),
	m_pSceneManager(nullptr),
	m_pD3D11Plugin(nullptr),
	m_pCamera(nullptr),
	m_pWorkspace(nullptr),
	m_bQuit(false)
{
}

void OgreGraphicsFramework::initialize()
{
	//Init
	m_pRoot = OGRE_NEW Ogre::Root();
	m_pD3D11Plugin = OGRE_NEW Ogre::D3D11Plugin();

	m_pRoot->installPlugin(m_pD3D11Plugin);

	if (!SetOgreConfig())
	{
		m_bQuit = true;
		return;
	}

	m_pRoot->initialise(false);

	// Creating window
	Ogre::uint32 width = 1280;
	Ogre::uint32 height = 720;
	Ogre::String sTitleName = "Game Engines Lecture 1";

	Ogre::NameValuePairList params;
	params.insert(std::make_pair("title", sTitleName));
	params.insert(std::make_pair("gamma", "true"));
	params.insert(std::make_pair("FSAA", "false"));
	params.insert(std::make_pair("vsync", "false"));

	m_pRenderWindow = Ogre::Root::getSingleton().createRenderWindow(sTitleName, width, height, false, &params);

	// Scene manager
	m_pSceneManager = m_pRoot->createSceneManager(Ogre::SceneType::ST_GENERIC, 1);

	// Camera
	m_pCamera = m_pSceneManager->createCamera("Main Camera");

	m_pCamera->setPosition(Ogre::Vector3(0, 10, 15));
	m_pCamera->lookAt(Ogre::Vector3(0, 0, 0));

	// Compositor Manager
	Ogre::CompositorManager2* compositorManager = m_pRoot->getCompositorManager2();

	const Ogre::String workspaceName("WorkSpace");

	if (!compositorManager->hasWorkspaceDefinition(workspaceName))
	{
		compositorManager->createBasicWorkspaceDef(workspaceName, Ogre::ColourValue::Blue);
	}

	m_pWorkspace = compositorManager->addWorkspace(m_pSceneManager, m_pRenderWindow->getTexture(), m_pCamera, workspaceName, true);

	// Create Entity
	Ogre::v1::Entity* ogreEntity = m_pSceneManager->createEntity("ogrehead.mesh");
	Ogre::SceneNode* ogreNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
	ogreNode->attachObject(ogreEntity);
}

bool OgreGraphicsFramework::SetOgreConfig()
{
	constexpr bool bAlwaysShowConfigWindow = true;
	if (bAlwaysShowConfigWindow || !m_pRoot->restoreConfig())
	{
		if (!m_pRoot->showConfigDialog())
		{
			return false;
		}
	}

	return true;
}

void OgreGraphicsFramework::update()
{
	if (m_pRenderWindow->isVisible())
		m_bQuit |= !m_pRoot->renderOneFrame();
}