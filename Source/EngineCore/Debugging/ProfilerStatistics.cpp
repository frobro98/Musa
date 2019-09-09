#include "ProfilerStatistics.hpp"
#include "MetricInterface.hpp"

struct ProfileMetric
{
	float64 totalMetricTimeMS;
	const tchar* filename;
	uint32 lineNumber;
	uint32 metricHitCount;
};

void CollectFrameMetrics()
{

}
