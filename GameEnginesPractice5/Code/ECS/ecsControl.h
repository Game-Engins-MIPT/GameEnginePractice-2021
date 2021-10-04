#pragma once
#include "flecs.h"
#include <OgreVector3.h>

struct Controllable {};

struct CameraPosition : public Ogre::Vector3
{
	using Ogre::Vector3::Vector3;
};

void register_ecs_control_systems(flecs::world* ecs);

