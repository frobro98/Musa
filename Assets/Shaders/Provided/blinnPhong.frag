#version 450 

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) out vec4 color;

/*layout(location = 0)*/ in vec3 norm;
/*layout(location = 1)*/ in vec3 lightDir;
/*layout(location = 2)*/ in vec3 viewDir;
/*layout(location = 3)*/ in vec2 texCoords;

// TODO - These variables need to be in a uniform buffer
// Material properties
vec3 diffuse_albedo = vec3(.5);
vec3 specular_albedo = vec3(.5);
float specular_power = 200.0;


layout(binding = 2) uniform sampler2D texSampler;

void main(void)
{
    // Normalize the incoming N, L and V vectors
    vec3 N = normalize(norm);
    vec3 L = normalize(lightDir);
    vec3 V = normalize(viewDir);
    vec3 H = normalize(L + V);

    // Compute the diffuse and specular components for each fragment
    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
    vec3 specular = pow(max(dot(N, H), 0.0), specular_power) * specular_albedo;

    // Write final color to the framebuffer
    color = texture(texSampler, texCoords) + vec4(diffuse + specular, 1.0);
}