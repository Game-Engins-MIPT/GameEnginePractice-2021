#pragma once
#include "flecs.h"
#include <OgreVector3.h>

struct Position : public Ogre::Vector3
{
	using Ogre::Vector3::Vector3;
};

struct Orientation : public Ogre::Quaternion
{
	using Ogre::Quaternion::Quaternion;
};

struct Velocity : public Ogre::Vector3
{
	using Ogre::Vector3::Vector3;
};

struct Gravity : public Ogre::Vector3
{
	using Ogre::Vector3::Vector3;
};

struct BouncePlane
{
	float x, y, z, w;
};

struct Bounciness
{
	float val;
};

struct ShiverAmount
{
	float val;
};

struct FrictionAmount
{
	float val;
};

typedef float Speed;

void register_ecs_phys_systems(flecs::world* ecs);

