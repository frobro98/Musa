#pragma once

#include "Musa.hs"

layout(location = 0) in vec3 fragpos;
layout(location = 1) in vec3 fragnorm;
layout(location = 2) in vec3 fragtan;
layout(location = 3) in vec2 fraguv;

struct MusaFSInput
{
	vec3 interpPos;
	vec3 interpNorm;
	vec3 interpTan;
	vec2 interpUV;
};

MusaFSInput GetInputs()
{
	MusaFSInput inputs;
	inputs.interpPos = fragpos;
	inputs.interpNorm = fragnorm;
	inputs.interpTan = fragtan;
	inputs.interpUV = fraguv;
	return inputs;
}
	

mat3 CalculateTBN()
{
	vec3 n = normalize(fragnorm);
	vec3 t = normalize(fragtan);
	vec3 b = normalize(cross(n, t));
	return mat3(t, b, n);
}