// COLOR RENDER - Vertex Shader
#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0)in vec4 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 uvCoords;
layout(location = 3)in vec3 color;

/*layout(location = 0)*/ out vec3 norm;
/*layout(location = 1)*/ out vec3 lightDir;
/*layout(location = 2)*/ out vec3 viewDir;
/*layout(location = 3)*/ out vec2 texCoords;

layout(binding = 0) uniform Transformations
{
	mat4 model;
	mat4 view;
	mat4 proj;
} transforms;

layout(binding = 1) uniform ViewInformation
{
	vec4 viewPos;
} viewInfo;

vec3 lightPos = vec3(15.0, 15.0, 15.0);

void main(void)
{              
	mat4 modelView = transforms.view * transforms.model;
    vec4 P = modelView * position;

    // Calculate normal in view-space
    norm = mat3(modelView) * normal;

    // Calculate light vector
    lightDir = lightPos - P.xyz;

    // Calculate view vector
    viewDir = viewInfo.viewPos.xyz - P.xyz;

    // Calculate the clip-space position of each vertex
    gl_Position = transforms.proj * P;

	texCoords = uvCoords;
}