#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) out vec4 color;

layout(location = 0) in vec3 FragPosition;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;


layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2) uniform MaterialProperties
{
	vec4 diffuse;
	float roughness;
	float metallic;
} materialProps;

layout(binding = 3) uniform ViewInformation
{
	vec4 viewPos;
} viewInfo;

void main(void)
{

	// TODO - These properties need to be tied into a uniform buffer
	// Material properties
	vec3 diffuse_albedo = vec3(.5);
	vec3 specular_albedo = vec3(.8);
	float specular_power = 128.0;
	vec3 ambient = vec3(.3);
	vec3 lightPos = vec3(0.0, 50.0, 65.0);
	vec3 viewPos = vec3(viewInfo.viewPos.xyz);//, -viewInfo.viewPos.z);

    // Normalize the incoming N, L and V vectors
    vec3 N = normalize(Normal);
    vec3 L = normalize(lightPos - FragPosition);
    vec3 V = normalize(viewPos - FragPosition);

	// Calculate R locally
    vec3 R = reflect(-L, N);
	
    // Compute the diffuse and specular components for each fragment
	
	float difference = max(dot(N, L), 0.0);
    vec3 diffuse = difference * vec3(1);// * vec3(texture(texSampler, TexCoords));
    
	float specularValue = pow(max(dot(R, V), 0.0), specular_power);
	vec3 specular = specular_albedo * specularValue;

    // Write final color to the framebuffer
	vec3 col = (ambient + diffuse + specular) * (materialProps.diffuse.rgb * vec3(.5));
    color = vec4(col, 1.0);
}