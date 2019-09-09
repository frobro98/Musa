
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
