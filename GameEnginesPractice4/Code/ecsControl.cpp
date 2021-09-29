#include "ecsControl.h"
#include "ecsSystems.h"
#include "ecsPhys.h"
#include "flecs.h"
#include "Input/InputHandler.h"

void register_ecs_control_systems(flecs::world &ecs)
{
  ecs.system<Velocity, const Speed, const Controllable>()
    .each([&](flecs::entity e, Velocity &vel, const Speed &spd, const Controllable &)
    {
      ecs.query<InputHandlerPtr>()
      .each([&](InputHandlerPtr input)
        {
          float deltaVel = 0.f;
          if (input.ptr->GetInputState().test(eIC_GoLeft))
            deltaVel -= spd;
          if (input.ptr->GetInputState().test(eIC_GoRight))
            deltaVel += spd;
          vel.x += deltaVel * e.delta_time();
        });
    });

  ecs.system<const Position, Velocity, const Controllable, const BouncePlane, const JumpSpeed>()
    .each([&](const Position &pos, Velocity &vel, const Controllable &, const BouncePlane &plane, const JumpSpeed &jump)
    {
      ecs.query<InputHandlerPtr>()
      .each([&](InputHandlerPtr input)
        {
          Ogre::Vector3 planeNorm(plane.x, plane.y, plane.z);
          constexpr float planeEpsilon = 0.1f;
          if (planeNorm.dotProduct(pos) < plane.w + planeEpsilon)
            if (input.ptr->GetInputState().test(eIC_Jump))
              vel.y = jump.val;
        });
    });
}

