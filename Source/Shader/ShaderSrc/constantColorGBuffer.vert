#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 uvCoords;

layout(location = 0) out vec3 outPosition;

layout(binding = 0) uniform Transformations
{
	mat4 model;
	mat4 view;
	mat4 projection;
};

void main(void)
{ 
	outPosition = (model * position).xyz;
	// Calculate the clip-space position of each vertex
    vec4 pos = projection * view * model * position;

	pos.y = -pos.y;
	pos.z = (pos.z + pos.w) / 2.0;
	gl_Position = pos;
}
