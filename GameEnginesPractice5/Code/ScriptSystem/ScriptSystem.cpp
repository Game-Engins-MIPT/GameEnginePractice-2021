#include "ScriptSystem.h"


ScriptSystem::ScriptSystem(InputHandler* pInputHandler, std::string strScriptsRoot) :
	m_pInputHandler(pInputHandler),
	m_strScriptsRoot(strScriptsRoot)
{

}

ScriptSystem::~ScriptSystem()
{

}

ScriptNode* ScriptSystem::CreateScriptNode(std::string strScriptName, flecs::entity entity)
{
	std::string strScriptPath = m_strScriptsRoot + strScriptName;

	ScriptNode* pScriptNode = new ScriptNode(strScriptPath, m_pInputHandler, entity);

	return pScriptNode;
}
