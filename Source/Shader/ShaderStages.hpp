#pragma once

enum ShaderStage
{
	Stage_Vert = 0,
	Stage_Frag = 1,
	Stage_Geom = 2,
	Stage_TessEval = 3,
	Stage_TessControl = 4,
	Stage_Comp = 5,

	Stage_Max = 6
};
static_assert(ShaderStage::Stage_Max == (ShaderStage::Stage_Comp + 1), "Max must be how many shader stages exist!");