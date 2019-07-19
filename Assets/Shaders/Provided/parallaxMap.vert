#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 uvCoords;

layout(location = 0) out vec3 FragPosition;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec2 TexCoords;
layout(location = 3) out vec3 Tangent;
layout(location = 4) out vec3 ViewDirection;

layout(binding = 0) uniform Transformations
{
	mat4 model;
	mat4 view;
	mat4 proj;
};


void main()
{
	FragPosition = vec3(model * position);
    Normal = (transpose(inverse(model)) * vec4(normal, 0)).xyz;
	Tangent = (transpose(inverse(model)) * vec4(tangent, 0)).xyz;
	TexCoords = uvCoords;
	ViewDirection = -(view * model * position).xyz;

    // Calculate the clip-space position of each vertex
    vec4 pos = proj * view * model * position;

	pos.y = -pos.y;
	pos.z = (pos.z + pos.w) / 2.0;
	gl_Position = pos;
}