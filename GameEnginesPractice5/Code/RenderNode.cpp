#include "RenderNode.h"

RenderNode::RenderNode(uint32_t idx) :
	m_nIdx(idx),
	m_pSceneNode(nullptr),
	m_bIsCamera(false)
{

}

RenderNode::RenderNode(uint32_t idx, Ogre::String& strMeshName) :
	m_nIdx(idx),
	m_pSceneNode(nullptr),
	m_strMeshName(strMeshName),
	m_bIsCamera(false)
{

}

RenderNode::~RenderNode()
{
	SAFE_DELETE(m_pSceneNode);
}

void RenderNode::SetId(uint32_t idx)
{
	m_nIdx = idx;
}

uint32_t RenderNode::GetId() const
{
	return m_nIdx;
}

Ogre::Vector3 RenderNode::GetPosition() const
{
	return m_vPosition;
}

void RenderNode::SetPosition(Ogre::Vector3 position)
{
	m_vPosition = position;
}

Ogre::Vector3 RenderNode::GetCameraPosition() const
{
	return m_vCameraPosition;
}

void RenderNode::SetCameraPosition(Ogre::Vector3 position)
{
	m_vCameraPosition = position;
}

Ogre::String& RenderNode::GetMeshName()
{
	return m_strMeshName;
}

void RenderNode::SetSceneNode(Ogre::SceneNode* pSceneNode)
{
	m_pSceneNode = pSceneNode;
}

Ogre::SceneNode* RenderNode::GetSceneNode() const
{
	return m_pSceneNode;
}

void RenderNode::EnableCamera(bool bEnableCamera)
{
	m_bIsCamera = bEnableCamera;
}

bool RenderNode::IsCameraEnabled() const
{
	return m_bIsCamera;
}