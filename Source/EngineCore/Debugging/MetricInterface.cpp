
#include "MetricInterface.hpp"
#include "Math/Vector2.hpp"
#include "Color.hpp"

static MetricTable metricTable;

MetricTable& GetMetricTable()
{
	return metricTable;
}
