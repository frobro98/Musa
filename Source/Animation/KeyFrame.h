#pragma once

#include "Platform.h"
#include "Malloc16Align.h"
#include "Math/Vector.h"
#include "Math/Quat.h"
#include "Time/Time.h"

struct FrameData : public Malloc16
{
	Vector translation;
	Quat rotation;
	Vector scale;
};

struct KeyFrame
{
	KeyFrame() = default;
	KeyFrame(uint32 boneCount_)
	{
		boneCount = boneCount_;
		boneFrameData = new FrameData[boneCount];
	}

	~KeyFrame()
	{
		delete[] boneFrameData;
	}

	FrameData* boneFrameData;
	uint32 boneCount;
	Time frameTime;
};