#pragma once

#include "Platform/Platform.hpp"
#include "Logging/LogSink.hpp"
#include "Logging/LogLevel.hpp"

// TODO - This is very much windows specific, so there needs to be some way to get at this without actually creating Win32Console stuff...
class CORE_API ConsoleWindowSink final : public LogSink
{
public:
	ConsoleWindowSink();
	~ConsoleWindowSink();

	virtual void OutputFormattedString(const LogLineEntry& entry) override;

private:
	HANDLE consoleOutHandle = INVALID_HANDLE_VALUE;
	u16 levelColors[LogLevel::Max];
};
