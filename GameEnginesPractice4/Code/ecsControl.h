#pragma once
#include "flecs.h"

struct Controllable {};
struct JumpSpeed { float val; };

void register_ecs_control_systems(flecs::world &ecs);

