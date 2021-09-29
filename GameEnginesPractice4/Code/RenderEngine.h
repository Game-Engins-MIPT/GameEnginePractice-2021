#pragma once

#include "Ogre.h"
#include "OgreRoot.h"
#include "OgreWindow.h"
#include "OgreItem.h"
#include "OgreMeshManager2.h"
#include "OgreMesh2.h"

#include "Compositor/OgreCompositorManager2.h"

#include "RenderSystems/Direct3D11/OgreD3D11Plugin.h"

#include "RenderThread.h"
#include "ResourceManager.h"

class RenderEngine
{
	friend class RenderThread;

public:
	RenderEngine(ResourceManager* pResourceManager);
	~RenderEngine();
	RenderEngine(const RenderEngine&) = delete;
	RenderEngine& operator=(const RenderEngine&) = delete;

	void Update();

	bool GetQuit() { return m_bQuit; }
	void SetQuit(bool bQuit) { m_bQuit = bQuit; }

  bool IsInited() { return m_bInited; }

	RenderThread* GetRT() const { return m_pRT; }

private:
	bool SetOgreConfig();
	
	void RT_Init();
	void RT_SetupDefaultCamera();
	void RT_SetupDefaultCompositor();
	void RT_LoadDefaultResources();
	void RT_SetupDefaultLight();
	void RT_OscillateCamera(float time);
	void RT_MoveOgreHead(float offset);
	void RT_SceneNodeSetPosition(Ogre::SceneNode* node, const Ogre::Vector3 &pos);
	void RT_LoadOgreHead();
	Ogre::SceneNode* RT_CreateSceneNode(const Ogre::String &mesh_name);

	Ogre::Root* m_pRoot;
	Ogre::Window* m_pRenderWindow;
	Ogre::SceneManager* m_pSceneManager;
	Ogre::Camera* m_pCamera;
	Ogre::CompositorWorkspace* m_pWorkspace;
	Ogre::D3D11Plugin* m_pD3D11Plugin;
	Ogre::SceneNode* m_pSceneNode;

	RenderThread* m_pRT;
	ResourceManager* m_pResourceManager;

	bool m_bQuit;
	bool m_bInited = false;
};

