#pragma once

layout(location = 0) in vec2 uv;

vec2 GetGBufferCoords()
{
	return uv;
}