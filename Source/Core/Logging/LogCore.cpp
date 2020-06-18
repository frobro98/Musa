
#include "LogCore.hpp"
#include "LoggingThread.hpp"
#include "File/DirectoryLocations.hpp"
#include "Sinks/ConsoleWindowSink.hpp"
#include "Sinks/DebugOutputWindowSink.hpp"
#include "Sinks/LogFileSink.hpp"

static Logger* logger = nullptr;

void InitializeLogger(LogLevel::Type level)
{
	Path logFilePath = Path(EngineLogPath()) / "musa.log";

	logger = new Logger;
	logger->logLevel = level;
	DynamicArray<LogSink*> logSinks(3);
	logSinks[0] = new ConsoleWindowSink;
	logSinks[1] = new DebugOutputWindowSink;
	logSinks[2] = new LogFileSink(logFilePath);
	logger->loggingThread = new LoggingThread(std::move(logSinks));
	logger->Log(LogLevel::Info, "Log Initialized!");
}

void DeinitializeLogger()
{
	delete logger;
	logger = nullptr;
}

Logger& GetLogger()
{
	Assert(logger);
	return *logger;
}

void Logger::AddLogSink(LogSink& sink)
{
	loggingThread->AddLogSink(sink);
}

void Logger::RemoveLogSink(LogSink& sink)
{
	loggingThread->RemoveSink(sink);
}

void Logger::PushLineToLog(const LogChannel& logChannel, LogLevel::Type level, const tchar* msg, size_t msgSize)
{
	// TODO - need to figure out if I don't format here or if I format here and then later not format
	LogLineEntry entry;
	entry.logMsg = String(msg, (u32)msgSize);
	entry.level = level;
	entry.logSlot = logChannel.logName;

	loggingThread->PushLogLine(entry);
}
