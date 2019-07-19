#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#define MAX_BONES 120

// Inputs
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvs;
//layout(location = 3) in vec4 color;
layout(location = 3) in vec4 weights;
layout(location = 4) in uvec4 boneIndices;

// Outputs
layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 norm;
layout(location = 2) out vec2 texCoords;

layout(binding = 0) uniform Transforms
{
	mat4 model;
	mat4 view;
	mat4 proj;
};

layout(binding = 4) uniform PoseInverses
{
	mat4 inverses[MAX_BONES];
};

layout(binding = 5) readonly buffer BoneDeltas
{
	mat4 boneWorlds[ ];
};

vec4 Skin(vec4 position)
{
	vec4 p0 = inverses[boneIndices.x] * position;
	p0 = boneWorlds[boneIndices.x] * p0;
	p0 = weights.x * p0;

	vec4 p1 = inverses[boneIndices.y] * position;
	p1 = boneWorlds[boneIndices.y] * p1;
	p1 = weights.y * p1;

	vec4 p2 = inverses[boneIndices.z] * position;
	p2 = boneWorlds[boneIndices.z] * p2;
	p2 = weights.z * p2;

	vec4 p3 = inverses[boneIndices.w] * position;
	p3 = boneWorlds[boneIndices.w] * p3;
	p3 = weights.w * p3;

	return p0 + p1 + p2 + p3;
}

void main()
{
	vec4 pos = Skin(position);
	norm = vec3(Skin(vec4(normal, 0.0)));

	mat4 modelView = view * model;
	fragPos = vec3(modelView * pos);
    //norm = mat3(transpose(inverse(modelView))) * norm;
	texCoords = uvs;

	pos = proj * vec4(fragPos, 1.0);

	pos.y = -pos.y;
	pos.z = (pos.z + pos.w) / 2.0;
	gl_Position = pos;
}
