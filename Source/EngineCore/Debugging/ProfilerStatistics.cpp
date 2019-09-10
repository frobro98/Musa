#include "ProfilerStatistics.hpp"
#include "MetricInterface.hpp"
#include "Debugging/MetricInterface.hpp"


void ProfilerStatistics::MarkBeginningOfFrame()
{
	GetMetricTable().SwapTablesForNewFrame();
}

void ProfilerStatistics::CollectAllFrameMetrics()
{
	[[maybe_unused]] const MetricTable::TableEntries& entries = GetMetricTable().GetCurrentTable();
}
