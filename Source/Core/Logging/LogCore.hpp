#pragma once

// Logging System Notes
// 
// Main log system gets all logging, uses fmtlib, can do color in console,
// writes to file, set log level, different "sinks"

// Design of log tool
// Needs to initialize specific "sinks" e.g. allocate console, debug window output,
// open file for writing, in-game console, etc.

#include "BasicTypes/Intrinsics.hpp"
#include "BasicTypes/UniquePtr.hpp"
#include "String/StringView.hpp"
#include "Containers/DynamicArray.hpp"
#include "fmt/format.h"
#include "CoreAPI.hpp"
#include "Logging/LogSlot.hpp"
#include "Logging/LogLevel.hpp"
#include "Logging/LogSink.hpp"

class LoggingThread;

DEFINE_LOG_SLOT(DefaultLog);

class CORE_API Logger
{
public:
	template <typename... Args>
	void Log(LogLevel level, const tchar* msg, Args&&... args)
	{
		Log(DefaultLog, level, msg, args...);
	}
	template <typename... Args>
	void Log(const LogSlot& logSlot, LogLevel level, const tchar* msg, Args&&... args)
	{
		if (ShouldLogAtLevel(level))
		{
			fmt::format_to(formatBuf, msg, args...);
			//LogToSinks(logSlot, formatBuf.data(), formatBuf.size());
			formatBuf.clear();
		}
	}

	forceinline bool ShouldLogAtLevel(LogLevel level)
	{
		return (u32)level <= (u32)logLevel;
	}

	forceinline void SetLogLevel(LogLevel level)
	{
		logLevel = level;
	}
	forceinline LogLevel GetLogLevel() const
	{
		return logLevel;
	}

	void AddLogSink(LogSink* sink);
	void RemoveLogSink(LogSink* sink);

private:
	//void LogToSinks(const LogSystemSlot& logSlot, const tchar* msg, size_t msgSize);

	friend CORE_API void InitializeLogger(LogLevel level);
private:
	// Threading!
	LoggingThread* loggingThread = nullptr;
	DynamicArray<UniquePtr<LogSink>> logOutputSinks;
	fmt::memory_buffer formatBuf;
	LogLevel logLevel;
};

CORE_API void InitializeLogger(LogLevel level);
CORE_API void DeinitializeLogger();
CORE_API Logger& GetLogger();


#define MUSA_LOG(LogSlot, level, msg, ...) GetLogger().Log(level, msg, _VA_ARGS_)

#define MUSA_DEBUG(LogSlot, msg, ...) MUSA_LOG(Logger::Level::Debug, msg, _VA_ARGS_)
#define MUSA_INFO(LogSlot, msg, ...) MUSA_LOG(Logger::Level::Info, msg, _VA_ARGS_)
#define MUSA_WARN(LogSlot, msg, ...) MUSA_LOG(Logger::Level::Warning, msg, _VA_ARGS_)
#define MUSA_ERR(LogSlot, msg, ...) MUSA_LOG(Logger::Level::Error, msg, _VA_ARGS_)
#define MUSA_FATAL(LogSlot, msg, ...) MUSA_LOG(Logger::Level::Fatal, msg, _VA_ARGS_)




