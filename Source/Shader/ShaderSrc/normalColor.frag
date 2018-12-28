#version 450 core
layout(location = 0)out vec4 fragColor;

in vec3 norm;

void main(void)
{
    fragColor = vec4(normalize(norm), 1.0);
}