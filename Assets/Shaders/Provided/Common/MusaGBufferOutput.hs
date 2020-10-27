#pragma once

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 position;
layout(location = 2) out vec4 normal;
layout(location = 3) out vec4 diffuseAlbedo;

struct GBufferOutData
{
	vec4 position;
	vec4 normal;
	vec4 diffuse;
};

void OutputToGBuffer(in GBufferOutData gbufferData)
{
	position = gbufferData.position;
	normal = gbufferData.normal;
	diffuseAlbedo = gbufferData.diffuse;
	outColor = gbufferData.diffuse;
}
