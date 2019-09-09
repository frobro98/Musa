#pragma once

#include <limits>
#include <intrin.h>

#include "Types/Intrinsics.hpp"
#include "Containers/StaticArray.hpp"
#include "Time/CyclePerformance.hpp"

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
	MetricType metricEventType;
};

constexpr uint64 MetricTableEntryCount = std::numeric_limits<uint16>::max();

struct MetricTable
{
	StaticArray<MetricEvent, MetricTableEntryCount> entries;
	uint32 numEntries = 0;
};

MetricTable& GetMetricTable();

//////////////////////////////////////////////////////////////////////////

// TODO - Think of better names for the metric recording interface

// TODO - HitCount isn't actually anything, so that needs to be rectified
// 	#define _BEGIN_TIMED_BLOCK_INFO_(Info, FileName, LineNum)	\
// 		do{														\
// 			Info.cycleCount = GetCycleCount();					\
// 			Info.filename = FileName;							\
// 			Info.lineCount = LineNum;							\
// 			Info.metricEventType = MetricType::BeginTimedMetric;	\
// 			++Info.hitCount;									\
// 		}while(false)

// #define _END_TIMED_BLOCK_INFO_(Info)					\
// 	do{													\
// 		MetricTable& table = GetMetricTable();			\
// 		Info.cycleCount = GetCycleCount() - Info.cycleCount;	\
// 		table.entries[table.numEntries++] = Info;		\
// 	} while (false)

using MetricID = uint32;

#define _BEGIN_TIMED_BLOCK_(Name, FileName, LineNum)					\
	do{																	\
		MetricTable& table = GetMetricTable();							\
		MetricEvent& metric##Name = table.entries[table.numEntries++];	\
		metric##Name.cycleCount = GetCycleCount();						\
		metric##Name.metricName = #Name;								\
		metric##Name.filename = FileName;								\
		metric##Name.lineCount = LineNum;								\
		metric##Name.metricEventType = MetricType::BeginTimedMetric;	\
		++metric##Name.hitCount;										\
	} while(false)
	
#define _END_TIMED_BLOCK_(Name, FileName, LineNum)		\
	do{													\
		MetricTable& table = GetMetricTable();							\
		MetricEvent& metric##Name = table.entries[table.numEntries++];	\
		metric##Name.cycleCount = GetCycleCount();						\
		metric##Name.metricName = #Name;								\
		metric##Name.filename = FileName;								\
		metric##Name.lineCount = LineNum;								\
		metric##Name.metricEventType = MetricType::EndTimedMetric;		\
	} while(false)

#define BEGIN_TIMED_BLOCK(Name) \
	_BEGIN_TIMED_BLOCK_(Name, __FILE__, __LINE__)

#define END_TIMED_BLOCK(Name) \
	_END_TIMED_BLOCK_(Name, __FILE__, __LINE__)

namespace Musa::Internal
{
	class MetricGroupCounter
	{
	public:
		static inline uint32 GetNewMetricID() { ++counter; Assertf(counter != 0, "Over ~4 billion metric groups defined!!"); return counter; }

	private:
		static uint32 counter;
	};

	class MetricCounter
	{
	public:
		static inline uint32 GetNewMetricID() { ++counter; Assertf(counter != 0, "Over ~4 billion metrics defined!!"); return counter; }

	private:
		static uint32 counter;
	};
}

#define METRIC_NAME(Name) Metric_##Name
#define METRIC_GROUP_NAME(Name) MetricGroup_##Name

#define DECLARE_METRIC_GROUP_STRUCT(Name, Counter)					\
	struct MetricGroup_##Name										\
	{																\
		static inline uint32 GetGroupID() { return Counter; }		\
		static inline const char* GetGroupName() { return #Name; }	\
	}

#define DECLARE_METRIC_STRUCT(Name, GroupName, Counter)		\
	struct Metric_##Name									\
	{														\
		using MetricGroup = METRIC_GROUP_NAME(GroupName);	\
		inline const char* GetName() const { return #Name; }\
		inline uint32 GetID() const { return Counter; }		\
	}

#define DECLARE_METRIC_GROUP(Name)															\
	DECLARE_METRIC_GROUP_STRUCT(Name, Musa::Internal::MetricGroupCounter::GetNewMetricID());

#define METRIC_STAT(Name, GroupName)														\
	DECLARE_METRIC_STRUCT(Name, GroupName, Musa::Internal::MetricCounter::GetNewMetricID()); \
	static MetricID Metric_##Name;

class ScopedTimeMetric
{
public:
	ScopedTimeMetric(MetricID id_, const tchar* filename, uint32 lineNumber)
		: id(id_)
	{
		_BEGIN_TIMED_BLOCK_(id, filename, lineNumber);
	}

	~ScopedTimeMetric()
	{
	}

private:
	MetricID id;
};

#define SCOPED_TIMED_BLOCK(Name)	\
	ScopedTimeMetric timedBlock_##Name(Metric_##Name, __FILE__, __LINE__);

