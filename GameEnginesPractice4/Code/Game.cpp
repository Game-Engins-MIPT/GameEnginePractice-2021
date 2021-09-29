#include "Game.h"
#include "ecsMesh.h"
#include "ecsSystems.h"
#include "ecsPhys.h"
#include "ecsControl.h"

Game::Game() :
	m_pRenderEngine(nullptr)
{
	m_pFileSystem = new FileSystem();
	m_pResourceManager = new ResourceManager(m_pFileSystem->GetMediaRoot());
	m_pInputHandler = new InputHandler(m_pFileSystem->GetMediaRoot());
	m_pRenderEngine = new RenderEngine(m_pResourceManager);

	m_Timer.Start();

  auto inputHandler = ecs.entity("inputHandler")
    .set(InputHandlerPtr{m_pInputHandler});
  auto renderEngine = ecs.entity("renderEngine")
    .set(RenderEnginePtr{m_pRenderEngine});

  register_ecs_mesh_systems(ecs);
  register_ecs_control_systems(ecs);
  register_ecs_phys_systems(ecs);

  auto ogreHeadControl = ecs.entity()
    .set(Position{0.f, 0.f, 0.f})
    .set(Velocity{0.f, 0.f, 0.f})
    .set(Speed{10.f})
    .set(FrictionAmount{0.9f})
    //.set(JumpSpeed{10.f})
    //.set(Gravity{0.f, -9.8065f, 0.f})
    //.set(BouncePlane{0.f, 1.f, 0.f, 0.f})
    //.set(Bounciness{0.3f})
    .set(MeshName{Ogre::String("ogrehead.mesh Imported")})
    .add<Controllable>();

  auto ogreHeadJump = ecs.entity()
    .set(Position{8.f, 0.f, 0.f})
    .set(Velocity{0.f, 5.f, 0.f})
    .set(Gravity{0.f, -9.8065f, 0.f})
    .set(BouncePlane{0.f, 1.f, 0.f, 0.f})
    .set(Bounciness{1.f})
    //.set(ShiverAmount{0.03f})
    .set(MeshName{Ogre::String("ogrehead.mesh Imported")});

  auto ogreHeadJumpFric = ecs.entity()
    .set(Position{-8.f, 0.f, -8.f})
    .set(Velocity{0.f, 5.f, 0.f})
    .set(Gravity{0.f, -9.8065f, 0.f})
    .set(BouncePlane{0.f, 1.f, 0.f, 0.f})
    .set(Bounciness{1.5f})
    .set(FrictionAmount{0.9f})
    .set(MeshName{Ogre::String("ogrehead.mesh Imported")});
}

Game::~Game()
{
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
	static float t = 0;
	t += m_Timer.DeltaTime();

	//m_pRenderEngine->GetRT()->RC_OscillateCamera(10.0f + sin(t));

  ecs.progress();
	return true;
}
