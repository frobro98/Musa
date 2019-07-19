#version 450

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvCoords;

layout(binding = 0) uniform Transformations
{
	mat4 model;
	mat4 view;
	mat4 projection;
};
 
vec4 VulkanProjectionModification(vec4 projectedPosition)
{
	projectedPosition.y = -projectedPosition.y;
	projectedPosition.z = (projectedPosition.z + projectedPosition.w) * 0.5f;
	return projectedPosition;
}

void main()
{
	vec4 pos = projection * view * model * vec4(inPos, 1.0);

	gl_Position = VulkanProjectionModification(pos);
} 