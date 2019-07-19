#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable


#define UNLIT_MODEL 1
#define LIT_MODEL 2
#define TOON_MODEL 3

layout(location = 0) in vec2 texCoords;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D positionGBuffer;
layout(binding = 1) uniform sampler2D normalGBuffer;
layout(binding = 2) uniform sampler2D albedoGBuffer;
//layout(binding = 3) uniform sampler2D shadowMapTexture;

struct Light
{
	vec4 position;
	vec4 direction;
	vec4 color;
	mat4 lightView;
	mat4 lightProjection;
};

layout(binding = 4) uniform ViewInformation
{
	vec4 viewPosition;
};

layout(binding = 5) uniform LightDescriptions
{
	Light lights;
};

const mat4 biasMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0 
);

vec4 VulkanProjectionModification(vec4 projectedPosition)
{
	projectedPosition.y = -projectedPosition.y;
	projectedPosition.z = (projectedPosition.z + projectedPosition.w) * 0.5f;
	return projectedPosition;
}

vec3 LightingModel(vec3 position, vec3 normal, vec4 albedo)
{
	const float specular_power = 128.0;
	vec3 L = normalize(lights.position.xyz - position);//normalize(lights.direction.xyz);
	vec3 N = normalize(normal);
	vec3 V = normalize(viewPosition.xyz - position);

	// Calculate R locally
	vec3 R = reflect(-L, N);

	float difference = max(dot(N, L), 0.0);
	vec3 diffuse = difference * albedo.rgb;
    
	float specularValue = pow(max(dot(R, V), 0.0), specular_power);
	vec3 specular = lights.color.rgb * specularValue;

	//vec4 shadowPos = lights.lightProjection * lights.lightView * fragPos;
	//shadowPos = VulkanProjectionModification(shadowPos);
	//shadowPos = biasMat * shadowPos;
	//float shadow = shadowProjection(shadowPos);

	// Write final color to the framebuffer
	return diffuse + specular;
}

vec3 ToonShadingModel(vec3 position, vec3 normal, vec4 albedo)
{
	vec3 L = normalize(lights.position.xyz - position);
	vec3 N = normalize(normal);

	float intensity = clamp(dot(L, N), 0, 1);

	vec3 color = vec3(0);
	if(intensity > 0.95f)
	{
		color = albedo.rgb;
	}
	else if(intensity > 0.5f)
	{
		color = vec3(.7, .7, .7) * albedo.rgb;
	}
	else if(intensity > 0.05f)
	{
		color = vec3(.35, .35, .35) * albedo.rgb;
	}
	else
	{
		color = vec3(.1, .1, .1) * albedo.rgb;
	}

	return color;
}

void main()
{
	vec4 fragPosSample = texture(positionGBuffer, texCoords);
	vec3 position = fragPosSample.xyz;
	uint shadingModel = uint(fragPosSample.w);

	vec3 normal = texture(normalGBuffer, texCoords).rgb;
	vec4 albedo = texture(albedoGBuffer, texCoords);

	vec3 col = albedo.rgb;
	if(shadingModel == LIT_MODEL)
	{
		col = LightingModel(position, normal, albedo);
	}
	else if(shadingModel == TOON_MODEL)
	{
		col = ToonShadingModel(position, normal, albedo);
	}

	//outColor = vec4(col * shadow, 1.0);
	outColor = vec4(col, 1.0);
}