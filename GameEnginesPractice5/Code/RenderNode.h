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

	uint32_t GetId() const;

	Ogre::Vector3 GetPosition() const;
	void SetPosition(Ogre::Vector3 position);

	Ogre::String& GetMeshName();
	void SetSceneNode(Ogre::SceneNode* pSceneNode);
	Ogre::SceneNode* GetSceneNode() const;

private:
	Ogre::Vector3 m_vPosition;

	uint32_t m_nIdx;
	Ogre::String m_strMeshName;
	Ogre::SceneNode* m_pSceneNode;
};

