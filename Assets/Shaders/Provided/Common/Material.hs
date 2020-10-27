#pragma once

#ifndef MAT_PROPS_BINDING
#define MAT_PROPS_BINDING 2
#endif // MAT_PROPS_BINDING

layout(binding = MAT_PROPS_BINDING) uniform MaterialProperties
{
	vec4 diffuse;
} materialProperties;