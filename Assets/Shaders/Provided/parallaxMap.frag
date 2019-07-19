#version 450 core

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 1) uniform sampler2D matTexture;
layout(binding = 2) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D displacementMap;

layout(binding = 4) uniform MaterialProperties
{
	vec4	  diffuse;
} materialProps;

layout(binding = 5) uniform ViewInformation
{
	vec4 viewPosition;
};

layout(location = 0) in vec3 FragPosition;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 ViewDirection;

layout(location = 0) out vec4 position;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 diffuseAlbedo;

void main()
{
	vec3 N = normalize(Normal);
	vec3 T = normalize(Tangent);
	vec3 B = normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	vec3 viewDirTanSpace = normalize(TBN * ViewDirection);

	float height = texture(displacementMap, TexCoords).r;
	float scale = 0.4f;
	float bias = -(scale/2.0);
	float heightsb = height * scale + bias;
	vec2 displacedUV = TexCoords + (heightsb * viewDirTanSpace.xy);

	vec3 sampledNormal = texture(normalMap, displacedUV).xyz;
	sampledNormal = normalize(sampledNormal * 2.0f - 1.0f);

	diffuseAlbedo = texture(matTexture, displacedUV);
	normal = vec4(sampledNormal, 0);
	position = vec4(FragPosition, 2);
}