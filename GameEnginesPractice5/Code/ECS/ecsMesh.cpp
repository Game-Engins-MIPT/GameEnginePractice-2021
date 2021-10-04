#include "ecsMesh.h"
#include "ecsSystems.h"
#include "ecsScript.h"
#include "ecsPhys.h"
#include "flecs.h"
#include "../RenderEngine.h"
#include "../ScriptSystem/ScriptNode.h"

void register_ecs_mesh_systems(flecs::world* ecs)
{
	ecs->system<ScriptNodeComponent, RenderNodeComponent, const CameraPosition, const Position>()
		.each([&](ScriptNodeComponent& scriptNode, RenderNodeComponent& renderNode, const CameraPosition& cameraPos, const Position& pos)
			{
				renderNode.ptr->SetPosition(pos);
				renderNode.ptr->SetCameraPosition(cameraPos);
				renderNode.ptr->EnableCamera();
			});
}

