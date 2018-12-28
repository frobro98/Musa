#pragma once

#include "PlatformDefinitions.h"

inline KeyboardState operator|=(KeyboardState& flag1, KeyboardState flag2)
{
	flag1 = (KeyboardState)((uint32)flag1 | (uint32)flag2);
	return flag1;
}

inline KeyStateFlags operator&(KeyStateFlags flag1, KeyStateFlags flag2)
{
	return (KeyStateFlags)((uint32)flag1 & (uint32)flag2);
}

// inline uint32 operator|(KeyStateFlags flag1, KeyStateFlags flag2)
// {
// 	return (uint32)flag1 | (uint32)flag2;
// }

inline KeyStateFlags operator|(KeyStateFlags flag1, KeyStateFlags flag2)
{
	return (KeyStateFlags)((uint32)flag1 | (uint32)flag2);
}

inline bool operator==(uint32 state, KeyInput flag)
{
	return state == static_cast<uint32>(flag);
}

