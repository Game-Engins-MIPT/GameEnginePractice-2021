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
	luabridge::LuaRef isStatic = properties[m_StaticsFieldName];

	bool bControllable = controllable.cast<bool>();
	bool bStatic = isStatic.cast<bool>();

	ent.set_name(properties[m_NameFieldName].cast<std::string>().c_str());

	if (bControllable)
		ent.add<Controllable>();

	if (bStatic)
		ent.add<Static>();
	
	luabridge::LuaRef camera = object[m_CameraFieldName];
	if (!camera.isNil())
	{
		Ogre::Vector3 vCameraPosition = GetCameraPosition();
		ent.set(CameraPosition{ vCameraPosition.x , vCameraPosition.y, vCameraPosition.z });
	}

	object[m_OnInitFunctionName]();

	Ogre::Vector3 vPosition = GetPosition();
	ent.set(Position{ vPosition.x , vPosition.y, vPosition.z });

	luabridge::LuaResult orientation = object[m_GetOrientationFunctionName]();
	if (orientation.wasOk())
	{
		Ogre::Quaternion vOrientation = GetOrientation();
		ent.set(Orientation{ vOrientation.w, vOrientation.x , vOrientation.y, vOrientation.z });
	}

}

ScriptNode::~ScriptNode()
{
	lua_close(m_script);
}

void ScriptNode::Update(float dt)
{
	
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

Ogre::Quaternion ScriptNode::GetOrientation() const
{
	luabridge::LuaRef object = luabridge::getGlobal(m_script, m_EntityFieldName);
	
	luabridge::LuaResult orientation = object[m_GetOrientationFunctionName]();
	assert(orientation.wasOk());

	Ogre::Quaternion vOrientation = orientation[0].cast<Ogre::Quaternion>();

	return vOrientation;
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
	luabridge::LuaRef parameters = object[m_ParametersFieldName];
	luabridge::Range parametersRange = luabridge::pairs(parameters);

	if (parameters.isNil())
	{
		return;
	}

	std::string strParameterName;
	float fValue;
	for (auto it = parametersRange.begin(); it != parametersRange.end(); ++it)
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
		.addFunction("__add", (Ogre::Vector3(Ogre::Vector3::*)(const Ogre::Vector3&) const) &Ogre::Vector3::operator+)
		.addFunction("__mul", (Ogre::Vector3(Ogre::Vector3::*)(const float) const) &Ogre::Vector3::operator*)
		.addProperty("x", &Ogre::Vector3::x, true)
		.addProperty("y", &Ogre::Vector3::y, true)
		.addProperty("z", &Ogre::Vector3::z, true)
		.endClass()
		.beginClass<Ogre::Radian>("Radian")
		.addConstructor<void(*) (float)>()
		.addFunction("__add", (Ogre::Radian(Ogre::Radian::*)(const Ogre::Radian&) const) &Ogre::Radian::operator+)
		.endClass()
		.beginClass<Ogre::Quaternion>("Quaternion")
		.addConstructor<void(*) (const Ogre::Radian&, const Ogre::Vector3&)>()
		.addFunction("setOrientation", &(Ogre::Quaternion::FromAngleAxis))
		.addFunction("__mul", (Ogre::Vector3(Ogre::Quaternion::*)(const Ogre::Vector3&) const) &Ogre::Quaternion::operator*)
		.endClass();

	luabridge::push(L, m_pInputHandler, ec);
	lua_setglobal(L, "inputHandler");
}