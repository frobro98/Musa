#include "EngineStatView.hpp"
#include "Debugging/ProfilerStatistics.hpp"
#include "Visualization/ScreenTextItem.hpp"
#include "Engine/FrameData.hpp"

static void MoveMetricPositionDown(Vector2& currentScreenPos)
{
	currentScreenPos.y += 20;
}

static void FormatAndAddTextItem(DynamicArray<ScreenTextItem>& textItems, Vector2& currentScreenPos, const char* metricName, float32 msTime)
{
	constexpr uint32 formattedStrLen = 256;
	tchar formattedString[formattedStrLen];
	snprintf(formattedString, formattedStrLen,
		"%s: %5.03fms"
		//" in %50s: line %4d"
		, metricName, msTime
		//, entry.filename, entry.lineCount
	);
	textItems.Add(ScreenTextItem{ formattedString, Color32::White(), currentScreenPos, .15f });
	MoveMetricPositionDown(currentScreenPos);
}

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
		Vector2 currentScreenPos(2, 2);
		const ProfiledFrameMark& frame = GetProfilingStatistics().GetPreviousFrame();

		FormatAndAddTextItem(textItems, currentScreenPos, "Frame Time", Frame::GetTickTimeSeconds() * 1000.f);

		MoveMetricPositionDown(currentScreenPos);

		for (auto& frameMetric : frame.frameMetrics)
		{
			FormatAndAddTextItem(textItems, currentScreenPos, frameMetric.metricName, (float32)frameMetric.totalMetricTimeMS);
		}
	}
}