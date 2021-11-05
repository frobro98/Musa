

layout(location = 0) in vec2 vertPos;
layout(location = 1) in vec2 vertUV;
layout(location = 2) in vec4 vertColor;

layout(binding = 0) uniform ImGuiUniforms
{
	vec2 scale;
	vec2 translate;
} transforms;

layout(location = 0) out struct
{
	vec4 color;
	vec2 uv;
} Out;

void main()
{
	Out.color = vertColor;
	Out.uv = vertUV;
	gl_Position = vec4(vertPos * transforms.scale + transforms.translate, 0, 1);
}