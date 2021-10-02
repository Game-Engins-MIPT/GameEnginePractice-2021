#include "ecsMesh.h"
#include "ecsSystems.h"
#include "ecsPhys.h"
#include "flecs.h"
#include "../RenderEngine.h"

void register_ecs_mesh_systems(flecs::world* ecs)
{
	static auto entityManagerQuery = ecs->query<EntityManagerPtr>();

	ecs->system<RenderNodeComponent, const Position>()
		.each([&](RenderNodeComponent& renderNode, const Position& pos)
			{
				entityManagerQuery.each([&](EntityManagerPtr em)
					{
						renderNode.pRenderNode->SetPosition(pos);
					});
			});
}

