#pragma once

#include "../RenderNode.h"
#include "flecs.h"
#include "ecsPhys.h"
#include "ecsMesh.h"
#include "ecsControl.h"

class EntityManager
{
public:
	EntityManager(RenderEngine* pRenderEngine, flecs::world* ecs);
	~EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;

	void CreateEntity(Ogre::String strMeshName, bool bControllable = false);

	std::unordered_map<uint32_t, RenderNode*> GetEntityQueue() const;

private:
	RenderEngine* m_pRenderEngine;
	flecs::world* m_pEcs;

	std::unordered_map<uint32_t, RenderNode*> m_entityQueue;

	uint32_t GetNewIndex() const;
};

