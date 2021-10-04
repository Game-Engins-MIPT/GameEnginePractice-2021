#include "ScriptSystem.h"


ScriptSystem::ScriptSystem(RenderEngine* pRenderEngine, InputHandler* pInputHandler, std::string strScriptsRoot, flecs::world* ecs) :
	m_pRenderEngine(pRenderEngine),
	m_pEcs(ecs),
	m_pInputHandler(pInputHandler),
	m_strScriptsRoot(strScriptsRoot)
{

}

ScriptSystem::~ScriptSystem()
{
	m_entityQueue.clear();
}

void ScriptSystem::CreateEntity(std::string strScriptName)
{
	flecs::entity newEntity = m_pEcs->entity();
	uint32_t nIndex = GetNewIndex();
	std::string strScriptPath = m_strScriptsRoot + strScriptName;

	ScriptNode* pScriptNode = new ScriptNode(strScriptPath, m_pInputHandler, newEntity);

	Ogre::String strMeshName = pScriptNode->GetMeshName();
	RenderNode* pRenderNode = new RenderNode(nIndex, strMeshName);

	newEntity.set(RenderNodeIndex{ nIndex })
		.set(RenderNodeComponent{ pRenderNode })
		.set(ScriptNodeComponent{ pScriptNode });

	m_pRenderEngine->GetRT()->RC_CreateSceneNode(pRenderNode);

	m_entityQueue[nIndex] = std::make_pair(pScriptNode, pRenderNode);
}

uint32_t ScriptSystem::GetNewIndex() const
{
	return m_entityQueue.size();
}

std::unordered_map<uint32_t, std::pair<ScriptNode*, RenderNode*>> ScriptSystem::GetEntityQueue() const
{
	return m_entityQueue;
}