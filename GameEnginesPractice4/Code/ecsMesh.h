#pragma once
#include "flecs.h"
#include "OgreString.h"

struct SceneNode
{
  class Ogre::SceneNode *pSceneNode;
};

struct MeshName
{
  Ogre::String name;
};

struct SceneNodeDeliveryIndex
{
  uint32_t idx;
};

void register_ecs_mesh_systems(flecs::world &ecs);

