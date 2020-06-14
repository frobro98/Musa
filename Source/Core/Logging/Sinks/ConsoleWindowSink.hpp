#pragma once

#include "Logging/LogSink.hpp"

// TODO - This is very much windows specific, so there needs to be some way to get at this without actually creating Win32Console stuff...
class ConsoleWindowSink : public LogSink
{
public:
	virtual void OutputFormattedString(const LogLineEntry& entry) override;

private:

};
