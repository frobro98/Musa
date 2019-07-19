#version 450 core

layout(binding = 2) uniform MaterialProperties
{
	vec4	  diffuse;
} materialProps;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 positionTexture;
layout(location = 1) out vec4 normalTexture;
layout(location = 2) out vec4 diffuseAlbedoTexture;

void main(void) 
{ 
	positionTexture = vec4(vec3(0), 1);
	normalTexture = vec4(0);
	diffuseAlbedoTexture = materialProps.diffuse;
}