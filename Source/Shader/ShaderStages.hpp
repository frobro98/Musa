// Copyright 2020, Nathan Blane

#pragma once

enum class ShaderStage : u32
{
	Vertex = 0,
	Fragment = 1,
	Geometry = 2,
	TessalationEval = 3,
	TessalationControl = 4,
	Compute = 5
};