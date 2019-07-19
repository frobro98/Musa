#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 1) uniform sampler2D matTexture;

layout(binding = 2) uniform MaterialProperties
{
	vec4	  diffuse;
} materialProps;

in vec2 texCoords;

layout(location = 0) out vec4 color;

void main()
{
	color = texture(matTexture, texCoords);// + materialProps.diffuse;
}