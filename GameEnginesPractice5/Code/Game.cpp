#include "Game.h"
#include "ECS/ecsMesh.h"
#include "ECS/ecsSystems.h"
#include "ECS/ecsPhys.h"
#include "ECS/ecsControl.h"
#include <stdlib.h>

Game::Game()
{
	m_pEcs = new flecs::world();
	m_pFileSystem = new FileSystem();
	m_pResourceManager = new ResourceManager(m_pFileSystem->GetMediaRoot());
	m_pInputHandler = new InputHandler(m_pFileSystem->GetMediaRoot());
	m_pRenderEngine = new RenderEngine(m_pResourceManager);
	m_pEntityManager = new EntityManager(m_pRenderEngine, m_pEcs);

	m_Timer.Start();

	m_pEcs->entity("inputHandler")
		.set(InputHandlerPtr{ m_pInputHandler });
	m_pEcs->entity("entityManager")
		.set(EntityManagerPtr{ m_pEntityManager });

	register_ecs_mesh_systems(m_pEcs);
	register_ecs_control_systems(m_pEcs);
	register_ecs_phys_systems(m_pEcs);

	m_pEntityManager->CreateEntity("ogrehead.mesh Imported");
	m_pEntityManager->CreateEntity("ogrehead.mesh Imported", true);
}

Game::~Game()
{
	SAFE_DELETE(m_pEcs);
	SAFE_DELETE(m_pFileSystem);
	SAFE_DELETE(m_pResourceManager);
	SAFE_DELETE(m_pInputHandler);
	SAFE_DELETE(m_pRenderEngine);
	SAFE_DELETE(m_pEntityManager);
}

void Game::Run()
{
	m_Timer.Reset();

	while (true)
	{
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
	m_pEcs->progress();
	return true;
}
