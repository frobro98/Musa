#pragma once

#ifndef PRIM_BINDING
#define PRIM_BINDING 0
#endif // PRIM_BINDING

layout(binding = PRIM_BINDING) uniform PrimProperties
{
	mat4 localWorld;
} prim;
