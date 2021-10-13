#include "RenderEngine.h"

#include "ProjectDefines.h"

RenderEngine::RenderEngine(ResourceManager* pResourceManager) :
	m_pRoot(nullptr),
	m_pRenderWindow(nullptr),
	m_pSceneManager(nullptr),
	m_pD3D11Plugin(nullptr),
	m_pCamera(nullptr),
	m_pWorkspace(nullptr),
	m_pRT(nullptr),
	m_bQuit(false),
	m_pResourceManager(pResourceManager)
{
	m_pRT = new RenderThread(this);

	m_pRT->RC_Init();
	m_pRT->RC_SetupDefaultCamera();
	m_pRT->RC_SetupDefaultCompositor();
	m_pRT->RC_LoadDefaultResources();
	m_pRT->RC_SetupDefaultLight();

	m_pRT->Start();
}

RenderEngine::~RenderEngine()
{
	SAFE_OGRE_DELETE(m_pRoot);
}

bool RenderEngine::SetOgreConfig()
{
#ifdef _DEBUG
	constexpr bool bAlwaysShowConfigWindow = true;
	if (bAlwaysShowConfigWindow || !m_pRoot->restoreConfig())
#else
	if (!m_pRoot->restoreConfig())
#endif
	{
		if (!m_pRoot->showConfigDialog())
		{
			return false;
		}
	}

	return true;
}

void RenderEngine::Update()
{
	Ogre::WindowEventUtilities::messagePump();

	for (RenderNode* pRenderNode : m_RenderNodes)
	{
		if (pRenderNode->GetStatic())
			continue;

		Ogre::Vector3 vPosition = pRenderNode->GetPosition();
		pRenderNode->GetSceneNode()->setPosition(vPosition);

		Ogre::Quaternion orientation = pRenderNode->GetOrientation();
		pRenderNode->GetSceneNode()->setOrientation(orientation);

		if (pRenderNode->IsCameraEnabled())
		{
			m_pCamera->setPosition(pRenderNode->GetCameraPosition());
			m_pCamera->lookAt(vPosition);
		}
	}

	if (m_pRenderWindow->isVisible())
		m_bQuit |= !m_pRoot->renderOneFrame();
}

void RenderEngine::RT_Init()
{
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
	Ogre::String sTitleName = "Game Engine";

	m_pRenderWindow = Ogre::Root::getSingleton().createRenderWindow(sTitleName, width, height, false);

	// Scene manager
	m_pSceneManager = m_pRoot->createSceneManager(Ogre::SceneType::ST_GENERIC, 1);
}

void RenderEngine::RT_SetupDefaultCamera()
{
	m_pCamera = m_pSceneManager->createCamera("Main Camera");

	m_pCamera->setPosition(Ogre::Vector3(0, 10, 35));
	m_pCamera->lookAt(Ogre::Vector3(0, 0, 0));
	m_pCamera->setNearClipDistance(0.2f);
	m_pCamera->setFarClipDistance(1000.0f);
	m_pCamera->setAutoAspectRatio(true);
}

void RenderEngine::RT_SetupDefaultCompositor()
{
	Ogre::CompositorManager2* compositorManager = m_pRoot->getCompositorManager2();

	const Ogre::String workspaceName("WorkSpace");

	if (!compositorManager->hasWorkspaceDefinition(workspaceName))
	{
		compositorManager->createBasicWorkspaceDef(workspaceName, Ogre::ColourValue::Blue);
	}

	m_pWorkspace = compositorManager->addWorkspace(m_pSceneManager, m_pRenderWindow->getTexture(), m_pCamera, workspaceName, true);
}

void RenderEngine::RT_LoadDefaultResources()
{
	m_pResourceManager->LoadOgreResources("resources.cfg");
}

void RenderEngine::RT_CreateSceneNode(RenderNode* pRenderNode)
{
	//Create an Item with the model we just imported.
	//Notice we use the name of the imported model. We could also use the overload
	//with the mesh pointer:
	Ogre::String strImportedMeshName = pRenderNode->GetMeshName() + "v1";
	if (!Ogre::MeshManager::getSingleton().resourceExists(strImportedMeshName))
		ImportV1Mesh(pRenderNode->GetMeshName());

	Ogre::Item* item = m_pSceneManager->createItem(strImportedMeshName,
		Ogre::ResourceGroupManager::
		AUTODETECT_RESOURCE_GROUP_NAME,
		Ogre::SCENE_DYNAMIC);
	Ogre::SceneNode* pSceneNode = m_pSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->
		createChildSceneNode(Ogre::SCENE_DYNAMIC);
	pSceneNode->attachObject(item);
	pSceneNode->scale(0.1f, 0.1f, 0.1f); // TODO: move out to ecs
	
	pRenderNode->SetSceneNode(pSceneNode);

	m_RenderNodes.push_back(pRenderNode);
}

void RenderEngine::ImportV1Mesh(Ogre::String strMeshName)
{
	//Load the v1 mesh. Notice the v1 namespace
	//Also notice the HBU_STATIC flag; since the HBU_WRITE_ONLY
	//bit would prohibit us from reading the data for importing.
	Ogre::String strImportedMeshName = strMeshName + "v1";

	Ogre::v1::MeshPtr v1Mesh;
	Ogre::MeshPtr v2Mesh;

	v1Mesh = Ogre::v1::MeshManager::getSingleton().load(
		strMeshName, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
		Ogre::v1::HardwareBuffer::HBU_STATIC, Ogre::v1::HardwareBuffer::HBU_STATIC);

	//Create a v2 mesh to import to, with a different name (arbitrary).
	v2Mesh = Ogre::MeshManager::getSingleton().createManual(
		strImportedMeshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	bool halfPosition = true;
	bool halfUVs = true;
	bool useQtangents = true;

	//Import the v1 mesh to v2
	v2Mesh->importV1(v1Mesh.get(), halfPosition, halfUVs, useQtangents);

	//We don't need the v1 mesh. Free CPU memory, get it out of the GPU.
	//Leave it loaded if you want to use athene with v1 Entity.
	v1Mesh->unload();
}

void RenderEngine::RT_SetupDefaultLight()
{
	// Directional lightning
	Ogre::Light* light = m_pSceneManager->createLight();
	Ogre::SceneNode* lightNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	light->setPowerScale(Ogre::Math::PI); //Since we don't do HDR, counter the PBS' division by PI
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3(-1, -1, -1).normalisedCopy());
}

