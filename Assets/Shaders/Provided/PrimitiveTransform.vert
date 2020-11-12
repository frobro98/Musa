
#define VIEW_BINDING 0
#include "Common/View.hs"

#include "Common/GLSLVertexOutput.hs"

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uvs;
layout(location = 2) in vec4 color;

layout(location = 0) out vec2 fraguv;
layout(location = 1) out vec4 fragcolor;

vec4 GetPrimitiveClipSpacePosition()
{
	vec4 clipPos = view.viewProj * view.worldView * vec4(position, 1);
	return VulkanProjectionModification(clipPos);
}

void main()
{
	fraguv = uvs;
	fragcolor = color;
	GLSLOutputPosition(GetPrimitiveClipSpacePosition());
}