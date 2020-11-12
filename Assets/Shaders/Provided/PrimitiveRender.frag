
layout(location = 0) in vec2 fraguv;
layout(location = 1) in vec4 fragcolor;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D mainTexture;

void main()
{
	outColor = fragcolor * texture(mainTexture, fraguv);
}