#include "RenderNode.h"

RenderNode::RenderNode(uint32_t idx) :
	m_nIdx(idx),
	m_pSceneNode(nullptr)
{

}

RenderNode::RenderNode(uint32_t idx, Ogre::String& strMeshName) :
	m_nIdx(idx),
	m_pSceneNode(nullptr),
	m_strMeshName(strMeshName)
{

}

RenderNode::~RenderNode()
{
	SAFE_DELETE(m_pSceneNode);
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