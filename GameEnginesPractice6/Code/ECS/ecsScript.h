#pragma once
#include "flecs.h"
#include "../ScriptSystem/ScriptNode.h"

struct ScriptNodeComponent
{
	class ScriptNode* ptr;
};

void register_ecs_script_systems(flecs::world* ecs);