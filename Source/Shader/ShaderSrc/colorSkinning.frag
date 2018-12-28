#version 450 core

layout(location = 0) out vec4 color;

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 texCoords;

layout(binding = 1) uniform sampler2D textureSampler;

layout(binding = 2) uniform MatProps
{
	vec4 diffuse;
} matProps;

layout(binding = 3) uniform ViewInformation
{
	vec4 viewPos;
} viewInfo;

void main()
{
		// TODO - These properties need to be tied into a uniform buffer
	// Material properties
	vec3 diffuse_albedo = vec3(.5);
	vec3 specular_albedo = vec3(.5);
	float specular_power = 128.0;
	vec3 ambient = vec3(0.1, 0.1, 0.1);
	vec3 lightPos = vec3(-10.0, 15.0, 0.0);

    // Normalize the incoming N, L and V vectors
    vec3 N = normalize(norm);
    vec3 L = normalize(lightPos - fragPos);
    vec3 V = normalize(viewInfo.viewPos.xyz - fragPos);

	// Calculate R locally
    vec3 R = reflect(-L, N);
	
    // Compute the diffuse and specular components for each fragment
	
	float difference = max(dot(N, L), 0.0);
    vec3 diffuse = diffuse_albedo * difference;
    
	float specularValue = pow(max(dot(R, V), 0.0), specular_power);
	vec3 specular = specular_albedo * specularValue;

    // Write final color to the framebuffer
    color = vec4(diffuse + specular, 1.0);
}