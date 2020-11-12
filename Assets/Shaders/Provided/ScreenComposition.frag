
layout(location = 0) in vec2 fraguvs;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D sceneColorTexture;
layout(binding = 1) uniform sampler2D userInterfaceTexture;

void main()
{
	// TODO - This operation represents a blend operation. This should be done by actually blending and not through a shader potentially
	vec4 sceneColor = texture(sceneColorTexture, fraguvs);
	vec4 uiColor = texture(userInterfaceTexture, fraguvs);
	outColor = sceneColor * (1 - uiColor.a) + uiColor;
}
