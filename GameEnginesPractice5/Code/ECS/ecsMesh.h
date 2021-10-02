#pragma once
#include "flecs.h"
#include "OgreString.h"

struct RenderNodeComponent
{
	class RenderNode* pRenderNode;
};

struct RenderNodeDeliveryIndex
{
	uint32_t idx;
};

void register_ecs_mesh_systems(flecs::world* ecs);

