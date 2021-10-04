#pragma once

#include "ScriptNode.h"

class ScriptSystem
{
public:
	ScriptSystem(RenderEngine* pRenderEngine, InputHandler* pInputHandler, std::string strScriptsRoot, flecs::world* ecs);
	~ScriptSystem();
	ScriptSystem(const ScriptSystem&) = delete;
	ScriptSystem& operator=(const ScriptSystem&) = delete;

	void CreateEntity(std::string strScriptName);

	std::unordered_map<uint32_t, std::pair<ScriptNode*, RenderNode*>> GetEntityQueue() const;

private:
	std::string m_strScriptsRoot;
	InputHandler* m_pInputHandler;
	RenderEngine* m_pRenderEngine;
	flecs::world* m_pEcs;

	std::unordered_map<uint32_t, std::pair<ScriptNode*, RenderNode*>> m_entityQueue;

	uint32_t GetNewIndex() const;
};

