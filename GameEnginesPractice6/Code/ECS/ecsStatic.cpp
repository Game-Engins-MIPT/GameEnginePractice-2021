#include "ecsStatic.h"
#include "ecsSystems.h"
#include "ecsScript.h"
#include "ecsPhys.h"
#include "flecs.h"
#include "../Input/InputHandler.h"

void register_ecs_static_systems(flecs::world* ecs)
{
	static auto renderSystemQuery = ecs->query<RenderNode>();

	ecs->system<const Static, RenderNodeComponent, ScriptNodeComponent>()
		.each([&](flecs::entity e, const Static&, RenderNodeComponent& renderNode, ScriptNodeComponent& scriptNode)
			{
				renderNode.ptr->SetStatic(scriptNode.ptr->GetIsStatic());
			});
}

