#pragma once

#include <limits>
#include <intrin.h>

#include "Types/Intrinsics.hpp"
#include "Containers/StaticArray.hpp"
#include "Time/CyclePerformance.hpp"

enum class MetricType : uint32
{
	TimedMetric,
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
#define _BEGIN_TIMED_BLOCK_INFO_(Info, FileName, LineNum)	\
	do{														\
		Info.cycleCount = GetCycleCount();					\
		Info.filename = FileName;							\
		Info.lineCount = LineNum;							\
		Info.metricEventType = MetricType::TimedMetric;		\
		++Info.hitCount;									\
	}while(false)

#define _END_TIMED_BLOCK_INFO_(Info)					\
	do{													\
		MetricTable& table = GetMetricTable();			\
		Info.cycleCount = GetCycleCount() - Info.cycleCount;	\
		table.entries[table.numEntries++] = Info;		\
	} while (false)

#define _BEGIN_TIMED_BLOCK_(Name, FileName, LineNum)			\
	MetricEvent metric##Name = {};								\
	do{															\
		metric##Name.cycleCount = GetCycleCount();				\
		metric##Name.metricName = #Name;						\
		metric##Name.filename = FileName;						\
		metric##Name.lineCount = LineNum;						\
		metric##Name.metricEventType = MetricType::TimedMetric;	\
		++metric##Name.hitCount;								\
	} while(false)
	
#define _END_TIMED_BLOCK_(Name, FileName, LineNum)		\
	do{													\
		MetricTable& table = GetMetricTable();			\
		metric##Name.cycleCount = GetCycleCount() - metric##Name.cycleCount;	\
		table.entries[table.numEntries++] = metric##Name;						\
	} while(false)

#define BEGIN_TIMED_BLOCK(Name) \
	_BEGIN_TIMED_BLOCK_(Name, __FILE__, __LINE__)

#define END_TIMED_BLOCK(Name) \
	_END_TIMED_BLOCK_(Name, __FILE__, __LINE__)

class ScopedTimeMetric
{
public:
	ScopedTimeMetric(const tchar* metricName, const tchar* filename, uint32 lineNumber)
	{
		event = {};
		event.metricName = metricName;
		_BEGIN_TIMED_BLOCK_INFO_(event, filename, lineNumber);

	}

	~ScopedTimeMetric()
	{
		// End timing block. 
		// Needs to have the exact time this is ended
		_END_TIMED_BLOCK_INFO_(event);
	}

private:
	MetricEvent event;
};

#define SCOPED_TIMED_BLOCK(Name)	\
	ScopedTimeMetric timedBlock_##Name(#Name, __FILE__, __LINE__);

#define TIMED_FUNCTION() \
	ScopedTimeMetric timedBlock_##Name(__func__, __FILE__, __LINE__);
