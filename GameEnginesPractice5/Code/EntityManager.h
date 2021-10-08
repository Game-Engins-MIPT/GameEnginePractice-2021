#pragma once

#include "ScriptSystem/ScriptSystem.h"

struct Entity
{
	RenderNode* pRenderNode;
	ScriptNode* pScriptNode;

	int idx;
};

class EntityManager
{
public:
	EntityManager(RenderEngine* pRenderEngine, ScriptSystem* pScriptSystem, flecs::world* ecs);
	~EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;

	void CreateEntity(std::string strScriptName);

	std::unordered_map<uint32_t, Entity> GetEntityQueue() const;

private:
	ScriptSystem* m_pScriptSystem;
	RenderEngine* m_pRenderEngine;
	flecs::world* m_pEcs;

	std::unordered_map<uint32_t, Entity> m_entityQueue;

	uint32_t GetNewIndex() const;
};