#pragma once

#include "Logging/LogSink.hpp"

// This sink pushes logs directly to the output debug window in Visual Studio
class DebugOutputWindowSink : public LogSink
{
public:
	virtual void OutputFormattedString(const LogLineEntry& entry) override;
private:

};

