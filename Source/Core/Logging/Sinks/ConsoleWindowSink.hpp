#pragma once

#include "Logging/LogSink.hpp"

class ConsoleWindowSink : public LogSink
{
public:
	virtual void OutputFormattedString(const LogEntry& entry) override;

private:

};
