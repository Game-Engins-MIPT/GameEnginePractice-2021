#pragma once

#include "ScriptNode.h"

class ScriptSystem
{
public:
	ScriptSystem(InputHandler* pInputHandler, std::string strScriptsRoot);
	~ScriptSystem();
	ScriptSystem(const ScriptSystem&) = delete;
	ScriptSystem& operator=(const ScriptSystem&) = delete;

	ScriptNode* CreateScriptNode(std::string strScriptName, flecs::entity entity);

private:
	std::string m_strScriptsRoot;
	InputHandler* m_pInputHandler;
};

