#include "EngineStatView.hpp"
#include "Debugging/ProfilerStatistics.hpp"
#include "Visualization/ScreenTextItem.hpp"

void EngineStatView::ToggleStats()
{
	statsVisible = !statsVisible;
}

bool EngineStatView::AreStatsVisible() const
{
	return statsVisible;
}

void EngineStatView::PushStatsToView(DynamicArray<ScreenTextItem>& textItems)
{
	if (statsVisible)
	{
		constexpr uint32 formattedStrLen = 256;
		Vector2 currentScreenPos(2, 2);
		const ProfiledFrameMark& frame = GetProfilingStatistics().GetPreviousFrame();

		for (auto& frameMetric : frame.frameMetrics)
		{
			tchar formattedString[formattedStrLen];
			snprintf(formattedString, formattedStrLen,
				"%s: %5.03fms"
				//" in %50s: line %4d"
				, frameMetric.metricName, frameMetric.totalMetricTimeMS
				//, entry.filename, entry.lineCount
			);
			textItems.Add(ScreenTextItem{ formattedString, Color32::White(), currentScreenPos, .15f });

			currentScreenPos.y += 20;
		}
	}
}