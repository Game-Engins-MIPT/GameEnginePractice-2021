#include "ecsMesh.h"
#include "ecsSystems.h"
#include "ecsScript.h"
#include "ecsPhys.h"

void register_ecs_script_systems(flecs::world* ecs)
{
	static auto scriptSystemQuery = ecs->query<ScriptSystemPtr>();

	ecs->system<ScriptNodeComponent, const Position>()
		.each([&](flecs::entity e, ScriptNodeComponent& scriptNode, const Position& pos)
			{
				scriptNode.ptr->Update(e.delta_time());
			});
}