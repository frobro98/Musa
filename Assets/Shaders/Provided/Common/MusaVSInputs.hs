#pragma once

#include "Musa.hs"
#include "GLSLVertexOutput.hs"

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 uvCoords;

vec3 GetWorldPosition()
{
	return vec3(prim.localWorld * position);
}

vec3 GetWorldNormal()
{
	return (transpose(inverse(prim.localWorld)) * vec4(normal, 0)).xyz;
}

vec3 GetWorldTangent()
{
	return (transpose(inverse(prim.localWorld)) * vec4(tangent, 0)).xyz;
}

vec2 GetUVCoordinates()
{
	return uvCoords;
}

vec4 GetClipSpacePosition()
{
	vec4 clipPos = view.viewProj * view.worldView * prim.localWorld * position;
	return VulkanProjectionModification(clipPos);
}

