
#include "Common/GLSLVertexOutput.hs"

vec2 GetVertexUVCoordinate(in uint vertexIndex)
{
	return vec2((vertexIndex << 1) & 2, vertexIndex & 2);
}

vec4 UVSpaceToClipSpace(in vec2 uvs)
{
	return vec4(uvs * 2.f - 1.f, 0.f, 1.f);
}


// Outputs
layout(location = 0) out vec2 fraguvs;

void main()
{
	fraguvs = GetVertexUVCoordinate(gl_VertexIndex);
	GLSLOutputPosition(UVSpaceToClipSpace(fraguvs));
}