#pragma once

#include "EngineCore/Types/Intrinsics.hpp"

namespace Musa
{
// External reference to group of components
struct Entity final
{
	const uint32 id;
	const uint32 version;
};

inline bool operator==(const Entity& lhs, const Entity& rhs)
{
	return lhs.id == rhs.id && lhs.version == rhs.version;
}

}

