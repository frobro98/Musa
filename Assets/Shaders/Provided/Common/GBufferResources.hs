#pragma once

layout(binding = 0) uniform sampler2D positionGBuffer;
layout(binding = 1) uniform sampler2D normalGBuffer;
layout(binding = 2) uniform sampler2D albedoGBuffer;

struct GBufferData
{
	vec3 position;
	vec3 normal;
	vec4 diffuse;
};

GBufferData GetGBufferData(vec2 gbufferInput)
{
	GBufferData data;
	data.position = texture(positionGBuffer, gbufferInput).xyz;
	data.normal = texture(normalGBuffer, gbufferInput).xyz;
	data.diffuse = texture(albedoGBuffer, gbufferInput);
	return data;
}
