#version 450 core

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
} transforms;

layout(binding = 4) uniform PoseInverse
{
	mat4 inverses[MAX_BONES];
} pose;

layout(binding = 5) uniform BoneDeltas
{
	mat4 anim[MAX_BONES];
} boneDeltas;

vec4 Skin(vec4 position)
{
	vec4 p0 = pose.inverses[boneIndices.x] * position;
	p0 = boneDeltas.anim[boneIndices.x] * p0;
	p0 = weights.x * p0;

	vec4 p1 = pose.inverses[boneIndices.y] * position;
	p1 = boneDeltas.anim[boneIndices.y] * p1;
	p1 = weights.y * p1;

	vec4 p2 = pose.inverses[boneIndices.z] * position;
	p2 = boneDeltas.anim[boneIndices.z] * p2;
	p2 = weights.z * p2;

	vec4 p3 = pose.inverses[boneIndices.w] * position;
	p3 = boneDeltas.anim[boneIndices.w] * p3;
	p3 = weights.w * p3;

	return p0 + p1 + p2 + p3;
}

void main()
{
	vec4 pos = Skin(position);
	norm = vec3(Skin(vec4(normal, 0.0)));

	mat4 modelView = transforms.view * transforms.model;
	fragPos = vec3(modelView * pos);
    //norm = mat3(transpose(inverse(modelView))) * norm;
	texCoords = uvs;

	pos = transforms.proj * vec4(fragPos, 1.0);

	pos.y = -pos.y;
	pos.z = (pos.z + pos.w) / 2.0;
	gl_Position = pos;
}
