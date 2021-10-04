#include "ScriptNode.h"

#include <algorithm>

ScriptNode::ScriptNode(std::string strScriptPath, InputHandler* pInputHandler, flecs::entity& ent) :
	m_pInputHandler(pInputHandler),
	m_strScriptPath(strScriptPath)
{
	m_script = luaL_newstate();
	luaL_openlibs(m_script);

	AddDependencies(m_script);

	luaL_dofile(m_script, m_strScriptPath.c_str());
	lua_pcall(m_script, 0, 0, 0);

	luabridge::LuaRef object = luabridge::getGlobal(m_script, m_EntityFieldName);
	luabridge::LuaRef properties = object[m_PropertiesFieldName];
	luabridge::LuaRef controllable = properties[m_ControllableFieldName];
	bool bControllable = controllable.cast<bool>();

	ent.set_name(properties[m_NameFieldName].cast<std::string>().c_str());

	if (bControllable)
		ent.add<Controllable>();
	
	luabridge::LuaRef camera = object[m_CameraFieldName];
	if (!camera.isNil())
	{
		Ogre::Vector3 vCameraPosition = GetCameraPosition();
		ent.set(CameraPosition{ vCameraPosition.x , vCameraPosition.y, vCameraPosition.z });
	}

	object[m_OnInitFunctionName]();

	Ogre::Vector3 vPosition = GetPosition();
	ent.set(Position{ vPosition.x , vPosition.y, vPosition.z });
}

ScriptNode::~ScriptNode()
{
	lua_close(m_script);
}

void ScriptNode::Update(float dt)
{
	ReloadScript();
	luabridge::LuaRef object = luabridge::getGlobal(m_script, m_EntityFieldName);
	object[m_OnUpdateFunctionName](dt);
}

Ogre::Vector3 ScriptNode::GetPosition() const
{
	luabridge::LuaRef object = luabridge::getGlobal(m_script, m_EntityFieldName);

	luabridge::LuaResult position = object[m_GetPositionFunctionName]();
	assert(position.wasOk());
	
	Ogre::Vector3 vPosition = position[0].cast<Ogre::Vector3>();

	return vPosition;
}

Ogre::Vector3 ScriptNode::GetCameraPosition() const
{
	luabridge::LuaRef object = luabridge::getGlobal(m_script, m_EntityFieldName);
	
	luabridge::LuaResult cameraPosition = object[m_GetCameraPositionFunctionName]();
	assert(cameraPosition.wasOk());

	Ogre::Vector3 vPosition = cameraPosition[0].cast<Ogre::Vector3>();

	return vPosition;
}

std::string ScriptNode::GetMeshName() const
{
	luabridge::LuaRef object = luabridge::getGlobal(m_script, m_EntityFieldName);

	luabridge::LuaRef properties = object[m_PropertiesFieldName];
	luabridge::LuaRef meshName = properties[m_MeshNameFieldName];
	return meshName.cast<std::string>();
}

void ScriptNode::ReloadScript()
{
	luabridge::LuaRef currentObject = luabridge::getGlobal(m_script, m_EntityFieldName);

	lua_State* script = luaL_newstate();
	luaL_openlibs(script);

	AddDependencies(script);

	luaL_dofile(script, m_strScriptPath.c_str());
	lua_pcall(script, 0, 0, 0);

	luabridge::LuaRef object = luabridge::getGlobal(script, m_EntityFieldName);
	luabridge::Range parameters = luabridge::pairs(object[m_ParametersFieldName]);

	std::string strParameterName;
	float fValue;
	for (auto it = parameters.begin(); it != parameters.end(); ++it)
	{
		strParameterName = it.key().cast<std::string>();
		fValue = it.value().cast<float>();
		currentObject[m_ParametersFieldName][strParameterName] = fValue;
	}
}

void ScriptNode::AddDependencies(lua_State* L)
{
	std::error_code ec;

	luabridge::getGlobalNamespace(L)
		.beginClass<InputHandler>("InputHandler")
		.addConstructor<void(*) (const std::string&)>()
		.addFunction("isCommandActive", &(InputHandler::IsCommandActive))
		.endClass()
		.beginClass<Ogre::Vector3>("Vector3")
		.addConstructor<void(*) (float, float, float)>()
		.addProperty("x", &Ogre::Vector3::x, true)
		.addProperty("y", &Ogre::Vector3::y, true)
		.addProperty("z", &Ogre::Vector3::z, true)
		.endClass();

	luabridge::push(L, m_pInputHandler, ec);
	lua_setglobal(L, "inputHandler");
}