#pragma once

#include "FNV-1a.h"

namespace Internal
{

}

using ComponentCtor = void(void*);
using ComponentDtor = void(void*);

template <typename Comp>
static constexpr const char* ComponentName()
{
	return __func__;
}

struct ComponentType
{
	ComponentCtor ctor;
	ComponentDtor dtor;
	uint64 typenameHash;
	uint16 size;
	uint16 alignment;
};

template <typename Comp>
inline ComponentType* MakeTypeFor()
{

}