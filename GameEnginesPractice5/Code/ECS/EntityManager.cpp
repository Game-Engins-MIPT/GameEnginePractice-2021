#include "EntityManager.h"


EntityManager::EntityManager(RenderEngine* pRenderEngine, flecs::world* ecs) :
	m_pRenderEngine(pRenderEngine),
	m_pEcs(ecs)
{

}

EntityManager::~EntityManager()
{
	m_entityQueue.clear();
}

void EntityManager::CreateEntity(Ogre::String strMeshName, bool bControllable)
{
	uint32_t nIndex = GetNewIndex();
	RenderNode* pRenderNode = new RenderNode(nIndex, strMeshName);

	auto newEntity = m_pEcs->entity()
		.set(Position{ 0.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 0.f, 0.f })
		.set(Speed{ 10.f })
		.set(FrictionAmount{ 0.9f })
		.set(JumpSpeed{ 20.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 0.f })
		.set(Bounciness{ 0.3f })
		.set(RenderNodeDeliveryIndex{ nIndex })
		.set(RenderNodeComponent{ pRenderNode });

	if (bControllable)
		newEntity.add<Controllable>();

	m_pRenderEngine->GetRT()->RC_CreateSceneNode(pRenderNode);

	m_entityQueue[nIndex] = pRenderNode;
}

uint32_t EntityManager::GetNewIndex() const
{
	return m_entityQueue.size();
}

std::unordered_map<uint32_t, RenderNode*> EntityManager::GetEntityQueue() const
{
	return m_entityQueue;
}