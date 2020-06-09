
#include "LogCore.hpp"
#include "Sinks/ConsoleWindowSink.hpp"
#include "Sinks/DebugOutputWindowSink.hpp"
#include "Sinks/LogFileSink.hpp"

static Logger* logger = nullptr;

void InitializeLogger(LogLevel level)
{
	logger = new Logger;
	logger->logLevel = level;
	auto& logSinks = logger->logOutputSinks;
	logSinks.Reserve(3);
	logSinks.Add(new ConsoleWindowSink);
	logSinks.Add(new DebugOutputWindowSink);
	logSinks.Add(new LogFileSink);
}

void DeinitializeLogger()
{
	delete logger;
}

Logger& GetLogger()
{
	Assert(logger);
	return *logger;
}
