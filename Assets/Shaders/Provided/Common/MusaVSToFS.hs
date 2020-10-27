#pragma once

layout(location = 0) out vec3 fragpos;
layout(location = 1) out vec3 fragnorm;
layout(location = 2) out vec3 fragtan;
layout(location = 3) out vec2 fraguv;

struct MusaVSOutput
{
	vec3 pos;
	vec3 norm;
	vec3 tan;
	vec2 uv;
};

void SetVSToFSOutput(in MusaVSOutput outputs)
{
	fragpos = outputs.pos;
	fragnorm = outputs.norm;
	fragtan = outputs.tan;
	fraguv = outputs.uv;
}