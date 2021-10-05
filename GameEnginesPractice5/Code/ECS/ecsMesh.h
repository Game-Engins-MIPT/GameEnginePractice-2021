#pragma once
#include "flecs.h"
#include "OgreString.h"

struct RenderNodeComponent
{
	class RenderNode* ptr;
};

struct RenderNodeIndex
{
	uint32_t idx;
};

void register_ecs_mesh_systems(flecs::world* ecs);

