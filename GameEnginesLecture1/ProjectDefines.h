#pragma once

#include "OgreMemoryAllocatorConfig.h"

#define SAFE_OGRE_DELETE(x) do { OGRE_DELETE(x); x = nullptr; } while(false)
