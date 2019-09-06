
#include "MetricInterface.hpp"
#include "RenderPipeline/UserInterfacePipeline.hpp"
#include "Math/Vector2.hpp"
#include "Color.hpp"

static MetricTable metricTable;

MetricTable& GetMetricTable()
{
	return metricTable;
}

void FormatMetricDisplay()
{
	Vector2 currentScreenPos(2, 2);
	for (uint32 i = 0; i < metricTable.numEntries; ++i)
	{
		const MetricEvent& entry = metricTable.entries[i];
		constexpr uint32 formattedStrLen = 256;
		tchar formattedString[formattedStrLen];
		snprintf(formattedString, formattedStrLen,
			"%s: %5.02fms"
			//" in %50s: line %4d"
			, entry.metricName, GetMillisecondsFrom(entry.cycleCount)
			//, entry.filename, entry.lineCount
		);
		AddTextToScreen(formattedString, .15f, currentScreenPos, Color32::White());

		currentScreenPos.y += 20;
	}

	metricTable.numEntries = 0;
}
