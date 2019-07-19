#version 450 core

layout(binding = 0) uniform Transformations
{
	mat4 model;
	mat4 view;
	mat4 proj;
} transforms;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 uvCoords;

out vec2 texCoords;

void main()
{
	texCoords = uvCoords;
	gl_Position = transforms.proj * transforms.view * transforms.model * position;
}