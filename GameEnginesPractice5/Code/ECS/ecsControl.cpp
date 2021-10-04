#include "ecsControl.h"
#include "ecsSystems.h"
#include "ecsScript.h"
#include "ecsPhys.h"
#include "flecs.h"
#include "../Input/InputHandler.h"

void register_ecs_control_systems(flecs::world* ecs)
{
	static auto inputQuery = ecs->query<InputHandlerPtr>();
	ecs->system<const Controllable, ScriptNodeComponent, CameraPosition, Position>()
		.each([&](flecs::entity e, const Controllable&, ScriptNodeComponent& scriptNode, CameraPosition& cameraPos, Position& pos)
			{
				Ogre::Vector3 vCameraPosition = scriptNode.ptr->GetCameraPosition();
				cameraPos.x = vCameraPosition.x;
				cameraPos.y = vCameraPosition.y;
				cameraPos.z = vCameraPosition.z;

				Ogre::Vector3 vPosition = scriptNode.ptr->GetPosition();
				pos.x = vPosition.x;
				pos.y = vPosition.y;
				pos.z = vPosition.z;
			});
}

