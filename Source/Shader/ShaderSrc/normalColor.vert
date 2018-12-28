#version 450

layout(location = 0)in vec4 position;
layout(location = 1)in vec3 normal;

out vec3 norm;

// Uniforms:
layout(binding = 0) uniform Transformations
{
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 proj;
} transforms;

void main(void)
{              
	// Remember: We use ROW major, but OpenGL GLSL multiplies in reverse order!
	vec4 pos = transforms.proj * transforms.view * transforms.model * position;

	pos.y = -pos.y;
	pos.z = (pos.z + pos.w) / 2.0;
	gl_Position = pos;

	// Position is the color + Grey
    norm = normal;
}