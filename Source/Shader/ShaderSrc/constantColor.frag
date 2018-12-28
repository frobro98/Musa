#version 450 core

layout(binding = 2) uniform MaterialProperties
{
	vec4	  diffuse;
} materialProps;

layout(location = 0) out vec4 color;

void main(void) 
{ 
	color = materialProps.diffuse;
}
