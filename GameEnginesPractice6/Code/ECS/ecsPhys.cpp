#include "ecsPhys.h"
#include <stdlib.h>

static float rand_flt(float from, float to)
{
	return from + (float(rand()) / RAND_MAX) * (to - from);
}

void register_ecs_phys_systems(flecs::world* ecs)
{
	ecs->system<Velocity, const Gravity, BouncePlane*, Position*>()
		.each([&](flecs::entity e, Velocity& vel, const Gravity& grav, BouncePlane* plane, Position* pos)
			{
				if (plane && pos)
				{
					Ogre::Vector3 planeNorm(plane->x, plane->y, plane->z);
					constexpr float planeEpsilon = 0.1f;
					if (planeNorm.dotProduct(*pos) < plane->w + planeEpsilon)
						return;
				}
				vel += grav * e.delta_time();
			});


	ecs->system<Velocity, Position, const BouncePlane, const Bounciness>()
		.each([&](Velocity& vel, Position& pos, const BouncePlane& plane, const Bounciness& bounciness)
			{
				Ogre::Vector3 planeNorm(plane.x, plane.y, plane.z);
				float dot = planeNorm.dotProduct(pos);
				if (dot < plane.w)
				{
					pos -= (dot - plane.w) * planeNorm;
					vel -= (1.f + bounciness.val) * planeNorm * planeNorm.dotProduct(vel);
				}
			});


	ecs->system<Velocity, const FrictionAmount>()
		.each([&](flecs::entity e, Velocity& vel, const FrictionAmount& friction)
			{
				vel -= vel * friction.val * e.delta_time();
			});


	ecs->system<Position, const Velocity>()
		.each([&](flecs::entity e, Position& pos, const Velocity& vel)
			{
				pos += vel * e.delta_time();
			});


	ecs->system<Position, const ShiverAmount>()
		.each([&](flecs::entity e, Position& pos, const ShiverAmount& shiver)
			{
				pos.x += rand_flt(-shiver.val, shiver.val);
				pos.y += rand_flt(-shiver.val, shiver.val);
				pos.z += rand_flt(-shiver.val, shiver.val);
			});
}

