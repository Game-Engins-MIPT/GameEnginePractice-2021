#include "ecsMesh.h"
#include "ecsSystems.h"
#include "ecsScript.h"
#include "ecsPhys.h"
#include "flecs.h"
#include "../RenderEngine.h"
#include "../ScriptSystem/ScriptNode.h"

void register_ecs_mesh_systems(flecs::world* ecs)
{
	ecs->system<RenderNodeComponent, const CameraPosition>()
		.each([&](RenderNodeComponent& renderNode, const CameraPosition& cameraPos)
			{
				renderNode.ptr->SetCameraPosition(cameraPos);
				renderNode.ptr->EnableCamera();
			});

	ecs->system<RenderNodeComponent, const Position>()
		.each([&](RenderNodeComponent& renderNode, const Position& pos)
			{
				renderNode.ptr->SetPosition(pos);
			});

	ecs->system<RenderNodeComponent, const Orientation>()
		.each([&](RenderNodeComponent& renderNode, const Orientation& orient)
			{
				renderNode.ptr->SetOrientation(orient);
			});
}

