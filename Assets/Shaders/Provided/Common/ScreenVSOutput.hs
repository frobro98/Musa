#pragma once

layout(location = 0) out vec2 uv;

vec2 GetScreenTextureCoords(in uint vertexIndex)
{
	return vec2((vertexIndex << 1) & 2, vertexIndex & 2);
}

vec4 TextureCoordsToClipSpace(in vec2 texCoords)
{
	return vec4(texCoords * 2.f - 1.f, 0.f, 1.f);
}

void OutputScreenQuad()
{
	uv = GetScreenTextureCoords(gl_VertexIndex);
	gl_Position = TextureCoordsToClipSpace(uv);
}
