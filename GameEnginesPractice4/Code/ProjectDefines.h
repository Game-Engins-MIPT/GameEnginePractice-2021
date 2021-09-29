#pragma once

#include "OgreMemoryAllocatorConfig.h"

typedef unsigned long threadID;

#define SAFE_OGRE_DELETE(x) { if(x) { OGRE_DELETE(x); (x) = nullptr; } }

#define ASSERT_NOT_IMPLEMENTED { OutputDebugStringA("Not implemented!\n"); __debugbreak(); }
