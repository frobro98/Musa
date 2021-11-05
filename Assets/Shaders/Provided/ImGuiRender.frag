
layout(location = 0) in struct
{
	vec4 color;
	vec2 uv;
} In;

layout(binding = 1) uniform sampler2D fontTex;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = In.color * texture(fontTex, In.uv);
}