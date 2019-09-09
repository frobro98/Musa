#pragma once

#include <limits>
#include <intrin.h>

#include "Types/Intrinsics.hpp"
#include "Containers/StaticArray.hpp"
#include "Containers/DynamicArray.hpp"
#include "Time/CyclePerformance.hpp"
#include "Utilities/MacroHelpers.hpp"

enum class MetricType : uint32
{
	BeginTimedMetric,
	EndTimedMetric,
	FrameMetric
};

struct MetricEvent
{
	// Represents the cycles it took the block to complete
	uint64 cycleCount;
	const tchar* metricName;
	const tchar* filename;
	uint32 lineCount;
	uint32 hitCount;
	uint32 metricID;
	MetricType metricEventType;

	friend bool operator==(const MetricEvent& e0, const MetricEvent& e1) { return e0.metricID == e1.metricID; }
};

constexpr uint64 MetricTableEntryCount = std::numeric_limits<uint16>::max();

struct MetricTable
{
	DynamicArray<MetricEvent> entries;
	uint32 numEntries = 0;
};

MetricTable& GetMetricTable();

//////////////////////////////////////////////////////////////////////////

// TODO - Think of better names for the metric recording interface

using MetricID = uint32;

namespace Musa::Internal
{
	class MetricGroupCounter
	{
	public:
		static uint32 GetNewMetricID() { ++counter; Assertf(counter != 0, "Over ~4 billion metric groups defined!!"); return counter; }

	private:
		static uint32 counter;
	};

	class MetricCounter
	{
	public:
		static uint32 GetNewMetricID() { ++counter; Assertf(counter != 0, "Over ~4 billion metrics defined!!"); return counter; }

	private:
		static uint32 counter;
	};
}

#define METRIC_NAME(Name) metric_##Name
#define METRIC_GROUP_NAME(Name) MetricGroup_##Name

#define DECLARE_METRIC_GROUP_STRUCT(Name)					\
	struct MetricGroup_##Name										\
	{																\
		static inline uint32 GetGroupID() { static const uint32 id = Musa::Internal::MetricGroupCounter::GetNewMetricID(); return id; }			\
		static inline const char* GetGroupName() { return #Name; }	\
	}

#define DECLARE_METRIC_STRUCT(Name, GroupName)		\
	struct Metric_##Name									\
	{														\
		using MetricGroup = METRIC_GROUP_NAME(GroupName);	\
		inline const char* GetName() { return #Name; }		\
		inline uint32 GetID() { static uint32 id = Musa::Internal::MetricGroupCounter::GetNewMetricID(); return id; }	\
	}

#define DECLARE_METRIC_GROUP(Name)															\
	DECLARE_METRIC_GROUP_STRUCT(Name);

#define METRIC_STAT(Name, GroupName)															\
	DECLARE_METRIC_STRUCT(Name, GroupName);	\
	static Metric_##Name metric_##Name;

#define _BEGIN_TIMED_BLOCK_(Name, id, FileName, LineNum)		\
	do{															\
		MetricTable& table = GetMetricTable();					\
		MetricEvent metric = {};								\
		metric.metricID = id;									\
		metric.cycleCount = GetCycleCount();					\
		metric.metricName = Name;								\
		metric.filename = FileName;								\
		metric.lineCount = LineNum;								\
		metric.metricEventType = MetricType::BeginTimedMetric;	\
		++metric.hitCount;										\
		table.entries.Add(metric);								\
		++table.numEntries;										\
	} while(false)

#define _END_TIMED_BLOCK_(id)									\
	do{															\
		MetricTable& table = GetMetricTable();					\
		MetricEvent metric = {};								\
		metric.metricID = id;									\
		metric.cycleCount = GetCycleCount();					\
		metric.metricName = nullptr;							\
		metric.filename = nullptr;								\
		metric.lineCount = 0;									\
		metric.metricEventType = MetricType::EndTimedMetric;	\
		table.entries.Add(metric);								\
		++table.numEntries;										\
	} while(false)

#define BEGIN_TIMED_BLOCK(Name) \
	_BEGIN_TIMED_BLOCK_(#Name, METRIC_NAME(Name).GetID(), __FILE__, __LINE__)

#define END_TIMED_BLOCK(Name) \
	_END_TIMED_BLOCK_(METRIC_NAME(Name).GetID());

class ScopedTimeMetric
{
public:
	ScopedTimeMetric(const char* name, uint32 id_, const tchar* filename, uint32 lineNumber)
		: id(id_)
	{
		_BEGIN_TIMED_BLOCK_(name, id, filename, lineNumber);
	}

	~ScopedTimeMetric()
	{
		_END_TIMED_BLOCK_(id);
	}

private:
	uint32 id;
};

#define SCOPED_TIMED_BLOCK(Name)	\
	ScopedTimeMetric timedBlock_##Name(#Name, metric_##Name.GetID(), __FILE__, __LINE__);

