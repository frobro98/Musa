
layout(location = 0)in vec4 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 uvCoords;

layout(location = 0) out vec3 FragPosition;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec2 TexCoords;

layout(binding = 0) uniform Transformations
{
	mat4 model;
	mat4 view;
	mat4 proj;
} transforms;

void main(void)
{             
	mat4 modelView = transforms.view * transforms.model;
	FragPosition = vec3(modelView * position);
    Normal = mat3(transpose(inverse(modelView))) * normal;
	TexCoords = uvCoords;

    // Calculate the clip-space position of each vertex
    vec4 pos = transforms.proj * vec4(FragPosition, 1.0);

	pos.y = -pos.y;
	pos.z = (pos.z + pos.w) / 2.0;
	gl_Position = pos;
}
