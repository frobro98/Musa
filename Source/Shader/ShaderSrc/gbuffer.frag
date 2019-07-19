#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

// NOTE - Using the "default.frag" behavior here, just to test out deferred rendering

layout(binding = 1) uniform sampler2D matTexture;

layout(binding = 2) uniform MaterialProperties
{
	vec4	  diffuse;
} materialProps;

layout(location = 0) in vec3 FragPosition;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;
layout(location = 3) in vec3 Tangent;

layout(location = 0) out vec4 position;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 diffuseAlbedo;

void main()
{
	vec3 N = normalize(Normal);
	vec3 T = normalize(Tangent);
	vec3 B = normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	position = vec4(FragPosition, 2);
	normal = vec4(Normal, 1);
	diffuseAlbedo = texture(matTexture, TexCoords) + materialProps.diffuse;
}