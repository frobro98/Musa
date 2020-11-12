#pragma once

vec4 VulkanProjectionModification(vec4 projectedPosition)
{
	projectedPosition.y = -projectedPosition.y;
	projectedPosition.z = (projectedPosition.z + projectedPosition.w) * .5f;
	return projectedPosition;
}

void GLSLOutputPosition(in vec4 clipPos)
{
	gl_Position = clipPos;
}