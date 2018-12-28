#version 450

layout (location = 0) in vec4 position;

layout(binding = 0) uniform Transformations
{
	mat4 model;
	mat4 view;
	mat4 proj;
} transforms;

void main(void)
{ 
	// Calculate the clip-space position of each vertex
    vec4 pos = transforms.proj * transforms.view * transforms.model * position;

	pos.y = -pos.y;
	pos.z = (pos.z + pos.w) / 2.0;
	gl_Position = pos;
}