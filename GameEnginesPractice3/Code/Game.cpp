#include "Game.h"

Game::Game() :
	m_pRenderEngine(nullptr)
{
	m_pFileSystem = new FileSystem();
	m_pResourceManager = new ResourceManager(m_pFileSystem->GetMediaRoot());
	m_pInputHandler = new InputHandler(m_pFileSystem->GetMediaRoot());
	m_pRenderEngine = new RenderEngine(m_pResourceManager);

	m_Timer.Start();
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
	
	float offset = 0.0f;
	if (m_pInputHandler->GetInputState().test(eIC_GoLeft))
		offset -= 1.0f;
	if (m_pInputHandler->GetInputState().test(eIC_GoRight))
		offset += 1.0f;
	offset *= m_Timer.DeltaTime();

	m_pRenderEngine->GetRT()->RC_OscillateCamera(10.0f + sin(t));
	m_pRenderEngine->GetRT()->RC_MoveOgreHead(offset);

	return true;
}
