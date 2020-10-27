#pragma once

#ifndef VIEW_BINDING
#define VIEW_BINDING 1
#endif // VIEW_BINDING

layout(binding = VIEW_BINDING) uniform ViewProperties
{
	mat4 worldView;
	mat4 viewProj;
	vec3 position;
} view;