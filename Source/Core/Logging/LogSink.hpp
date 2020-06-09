#pragma once

#include "BasicTypes/Intrinsics.hpp"

struct LogEntry;

class LogSink
{
public:
	virtual void OutputFormattedString(const LogEntry& entry) = 0;
};
