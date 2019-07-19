// COLOR RENDER - Vertex Shader
#version 450 core

layout(location = 0) in vec4 position;

out vec4 color;

// Uniforms:
layout(binding = 0) uniform Transformations
{
	mat4 model;
	mat4 view;
	mat4 proj;
} transforms;

void main(void)
{              
	// Remember: We use ROW major, but OpenGL GLSL multiplies in reverse order!
    gl_Position = transforms.proj * transforms.view * transforms.model * position;    

	// Position is the color + Grey
    color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
}