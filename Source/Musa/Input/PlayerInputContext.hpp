// Copyright 2020, Nathan Blane

#pragma once
#include "BasicTypes/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"
#include "String/String.h"
#include "String/StringView.hpp"
#include "Input/InputDefinitions.hpp"

struct InputRange
{
	f32 minRawRange;
	f32 maxRawRange;
	f32 minNormalizedRange;
	f32 maxNormalizedRange;
};

struct SingleInput
{
	String inputName;
	Inputs::Type type;
};

struct RangedInput
{
	InputRange range;
	SingleInput input;
};

struct InputRangeValue
{
	const SingleInput* input;
	f32 rangeValue;
};

struct PlayerInputContext
{
	String contextName;
	DynamicArray<SingleInput> inputActions;
	DynamicArray<SingleInput> inputStates;
	DynamicArray<RangedInput> inputRanges;
	u32 nameHash;

	friend bool operator==(const PlayerInputContext& pc0, const PlayerInputContext& pc1)
	{
		return pc0.nameHash == pc1.nameHash;
	}
};

PlayerInputContext MakeInputContext(const StringView& name);

