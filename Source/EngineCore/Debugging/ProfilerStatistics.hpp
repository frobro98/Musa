#pragma once

#include "Types/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"
#include "Containers/StaticArray.hpp"

struct ProfileMetric
{
	float64 totalMetricTimeMS;
	const tchar* metricName;
	const tchar* filename;
	uint32 lineNumber;
	uint32 metricHitCount;
	uint32 frameNumber;
};

struct ProfiledFrameMark
{
	void Clear()
	{
		frameMetrics.Clear();
	}

	//Map<MetricGroup, DynamicArray<ProfileMetric>> groupMetrics;
	DynamicArray<ProfileMetric> frameMetrics;
};

class ProfilerStatistics
{
public:
	// Go to the next frame of data
	void ProfileFrameIncrement();
	void CollectAllFrameMetrics();
	const ProfiledFrameMark& GetPreviousFrame() const;

private:
	// 3 seconds of data for a 60fps locked game
	static constexpr uint32 frameProfileCount = 300;
	StaticArray<ProfiledFrameMark, frameProfileCount> frameProfiles = {};
	uint32 currentFrameIndex = 0;
};

ProfilerStatistics& GetProfilingStatistics();
