// Copyright 2020, Nathan Blane

#include "MetricInterface.hpp"
#include "Math/Vector2.hpp"
#include "Color.hpp"

namespace Musa::Internal
{
	uint32 MetricGroupCounter::counter = 0;
	uint32 MetricCounter::counter = 0;
}

static MetricTable metricTable;

MetricTable& GetMetricTable()
{
	return metricTable;
}

void MetricTable::AddMetric(const MetricEvent& event)
{
	entries[currentTableIndex].Add(event);
}

const MetricTable::TableEntries& MetricTable::GetCurrentTable() const
{
	// TODO: insert return statement here
	return entries[currentTableIndex];
}

void MetricTable::SwapTablesForNewFrame()
{
	currentTableIndex = currentTableIndex == 0 ? 1 : 0;
	entries[currentTableIndex].Clear();
}
