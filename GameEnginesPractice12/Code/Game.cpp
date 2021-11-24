#include "Game.h"
#include "ECS/ecsMesh.h"
#include "ECS/ecsSystems.h"
#include "ECS/ecsPhys.h"
#include "ECS/ecsControl.h"
#include <stdlib.h>
#include "optick.h"

Game::Game()
{
	m_pEcs = new flecs::world();
	m_pFileSystem = new FileSystem();
	m_pResourceManager = new ResourceManager(m_pFileSystem->GetMediaRoot());
	m_pInputHandler = new InputHandler(m_pFileSystem->GetMediaRoot());
	m_pRenderEngine = new RenderEngine(m_pResourceManager);
	m_pScriptSystem = new ScriptSystem(m_pInputHandler, m_pFileSystem->GetScriptsRoot());
	m_pEntityManager = new EntityManager(m_pRenderEngine, m_pScriptSystem, m_pEcs);
	m_pLoadingSystem = new LoadingSystem(m_pEntityManager, m_pFileSystem->GetSavesRoot());

	m_Timer.Start();

	m_pEcs->entity("inputHandler")
		.set(InputHandlerPtr{ m_pInputHandler });
	m_pEcs->entity("scriptSystem")
		.set(ScriptSystemPtr{ m_pScriptSystem });

	m_pLoadingSystem->LoadFromXML("initialScene.xml");

  for (int i = 0; i < 100; ++i)
  {
    float rndVal = float(rand()) / RAND_MAX;
    m_pEntityManager->CreateMeshEntity("ogrehead.mesh")
      .set(Position{(rndVal - 0.5f) * 40.f, 0.f, -8.f})
      .set(Velocity{0.f, 30.f, 0.f})
      .set(Gravity{0.f, -9.8065f, 0.f})
      .set(BouncePlane{0.f, 1.f, 0.f, -10.f})
      .set(Bounciness{1.5f})
      .set(FrictionAmount{0.9f + float(rand()) / RAND_MAX * 0.1f});
  }

	register_ecs_mesh_systems(m_pEcs);
	register_ecs_control_systems(m_pEcs);
	register_ecs_phys_systems(m_pEcs);
	register_ecs_script_systems(m_pEcs);
	register_ecs_static_systems(m_pEcs);
}

Game::~Game()
{
	SAFE_DELETE(m_pEcs);
	SAFE_DELETE(m_pFileSystem);
	SAFE_DELETE(m_pResourceManager);
	SAFE_DELETE(m_pInputHandler);
	SAFE_DELETE(m_pRenderEngine);
	SAFE_DELETE(m_pScriptSystem);
	SAFE_DELETE(m_pEntityManager);
	SAFE_DELETE(m_pLoadingSystem);
}

void Game::Run()
{
	m_Timer.Reset();

	while (true)
	{
    OPTICK_FRAME("MainThread");
		m_pRenderEngine->GetRT()->RC_BeginFrame();

		m_Timer.Tick();

		if (m_pInputHandler)
			m_pInputHandler->Update();

		if (!Update())
			break;

		m_pRenderEngine->GetRT()->RC_EndFrame();
	}
}

bool Game::Update()
{
  OPTICK_EVENT();
	m_pEcs->progress();
	return true;
}
