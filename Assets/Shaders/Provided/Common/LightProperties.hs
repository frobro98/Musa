#pragma once

#ifndef LIGHT_BINDING
#define LIGHT_BINDING 4
#endif // LIGHT_BINDING

layout(binding = LIGHT_BINDING) uniform LightProperties
{
	vec4 position;
	vec4 direction;
	vec4 color;
	mat4 lightView;
	mat4 lightProjection;
} light;

const mat4 biasMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0 
);

float LinearizeDepth(float depth)
{
	float n = .1;
	float f = 1000.0;
	float z = depth * 2.0 - 1.0;
	return ((2.0 * n * f) / (f + n - z * (f - n))) / f;
}

float shadowProjection(vec4 lightSpacePosition)
{
	float shadow = 1;
//	vec4 shadowCoords = lightSpacePosition / lightSpacePosition.w;
//	shadowCoords.st = shadowCoords.st *.5 + .5;
//
//	float depthValue = texture(shadowMapTexture, shadowCoords.st).r;
//	float shadowProjValue = shadowCoords.z;
//
//	if (shadowCoords.z > -1.0 && shadowCoords.z < 1.0)
//	{
//		if(depthValue < shadowProjValue)
//		{
//			shadow = .25f;
//		}
//	}
	return shadow;
}
