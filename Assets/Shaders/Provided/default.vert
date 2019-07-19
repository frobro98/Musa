#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 uvCoords;
//layout(location = 4) in vec4 color;

layout(binding = 0) uniform Transformations
{
	mat4 model;
	mat4 view;
	mat4 proj;
} transforms;

out vec2 texCoords;

void main()
{
	texCoords = uvCoords;
	vec4 pos = transforms.proj * transforms.view * transforms.model * vec4(position.xyz, 1.0);

	pos.y = -pos.y;
	pos.z = (pos.z + pos.w) / 2.0;
	gl_Position = pos;
}