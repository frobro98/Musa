#version 450

// TODO - figure out uniform buffers in Vulkan

layout(binding = 2) uniform sampler2D matTexture;

layout(binding = 3) uniform MaterialProperties
{
	vec4	  diffuse;
} materialProps;

in vec2 texCoords;

out vec4 color;

void main()
{
	color = texture(matTexture, texCoords);
}