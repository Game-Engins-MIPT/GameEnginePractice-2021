#include "Game.h"

Game::Game() :
	m_pRenderEngine(nullptr)
{
	m_pRenderEngine = new RenderEngine();

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

		if (!Update())
			break;

		m_pRenderEngine->GetRT()->RC_EndFrame();
	}
}

bool Game::Update()
{
	static float t = 0;
	// t += m_Timer.DeltaTime();
	t += 0.001f;
	m_pRenderEngine->GetRT()->RC_OscillateCamera(10.0f + sin(t));

	return true;
}
