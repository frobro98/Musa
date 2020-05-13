#pragma once

#include "Platform.h"
#include "Malloc16Align.h"
#include "Math/Vector4.hpp"
#include "Math/Quat.hpp"
#include "Time/Time.h"

// TODO - Refactor the hell out of these structures...
struct FrameData : public Malloc16
{
	Vector4 translation;
	Quat rotation;
	Vector4 scale;
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
// 
// forceinline void Deserialize(DeserializeBase& ser, KeyFrame& keyframe)
// {
// 	ser.DeserializeData(&keyframe, sizeof(KeyFrame));
// }

