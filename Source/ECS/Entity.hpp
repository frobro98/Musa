#pragma once

#include "EngineCore/Types/Intrinsics.hpp"


namespace Musa
{

// External reference to group of components
struct Entity final
{
	uint32 id;
	uint32 version;
};

inline bool operator==(const Entity& lhs, const Entity& rhs)
{
	return lhs.id == rhs.id && lhs.version == rhs.version;
}

}

