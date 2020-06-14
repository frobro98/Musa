#pragma once

#include "BasicTypes/Intrinsics.hpp"

struct LogLineEntry;

class LogSink
{
public:
	virtual void OutputFormattedString(const LogLineEntry& entry) = 0;
};
