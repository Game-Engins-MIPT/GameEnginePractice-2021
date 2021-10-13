#pragma once
#include <cstdint>

#include "OgreSceneNode.h"
#include "RenderEngine.h"

class RenderNode
{
public:
	RenderNode() = delete;
	RenderNode(uint32_t idx);
	RenderNode(uint32_t idx, Ogre::String& strMeshName);
	~RenderNode();

	void SetId(uint32_t idx);
	uint32_t GetId() const;

	Ogre::Vector3 GetPosition() const;
	void SetPosition(Ogre::Vector3 position);

	Ogre::Vector3 GetCameraPosition() const;
	void SetCameraPosition(Ogre::Vector3 position);

	Ogre::Quaternion GetOrientation() const;
	void SetOrientation(Ogre::Quaternion position);

	void EnableCamera(bool bEnableCamera = true);
	bool IsCameraEnabled() const;

	Ogre::String& GetMeshName();
	void SetSceneNode(Ogre::SceneNode* pSceneNode);
	Ogre::SceneNode* GetSceneNode() const;

	void SetStatic(bool isStatic);
	bool GetStatic() const;
private:
	Ogre::Vector3 m_vPosition;
	Ogre::Vector3 m_vCameraPosition;
	Ogre::Quaternion m_vOrientation;

	uint32_t m_nIdx;
	Ogre::String m_strName;
	Ogre::String m_strMeshName;
	Ogre::SceneNode* m_pSceneNode;

	bool m_bIsCamera;
	bool m_bIsStatic;
	void Init();
};

