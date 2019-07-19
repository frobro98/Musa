#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 1) uniform sampler2D matTexture;

layout(binding = 2) uniform MaterialProperties
{
	vec4	  diffuse;
} materialProps;

in vec2 texCoords;

layout(location = 0) out vec4 positionTexture;
layout(location = 1) out vec4 normalTexture;
layout(location = 2) out vec4 diffuseAlbedoTexture;

void main()
{
	positionTexture = vec4(0);
	normalTexture = vec4(0);
	diffuseAlbedoTexture = texture(matTexture, texCoords);
}