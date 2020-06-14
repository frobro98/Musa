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
#include "Logging/LogChannel.hpp"
#include "Logging/LogLevel.hpp"
#include "Logging/LogSink.hpp"

class LoggingThread;

DEFINE_LOG_CHANNEL(DefaultLog);

class CORE_API Logger
{
public:
	~Logger();

	template <typename... Args>
	forceinline void LogFormat(LogLevel level, const tchar* msg, Args&&... args)
	{
		LogFormat(DefaultLog, level, msg, args...);
	}
	template <typename... Args>
	forceinline void LogFormat(const LogChannel& logSlot, LogLevel level, const tchar* msg, Args&&... args)
	{
		if (ShouldLogAtLevel(level))
		{
			fmt::format_to(formatBuf, msg, args...);

			formatBuf.clear();
		}
	}

	forceinline void Log(LogLevel level, const tchar* msg)
	{
		Log(DefaultLog, level, msg);
	}

	forceinline void Log(const LogChannel& logChannel, LogLevel level, const tchar* msg)
	{
		if (ShouldLogAtLevel(level))
		{
			PushLineToLog(logChannel, logLevel, msg, Strlen(msg));
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

	void AddLogSink(LogSink& sink);
	void RemoveLogSink(LogSink& sink);

private:
	void PushLineToLog(const LogChannel& logChannel, LogLevel level, const tchar* msg, size_t msgSize);

	friend CORE_API void InitializeLogger(LogLevel level);
private:
	// Threading!
	LoggingThread* loggingThread = nullptr;
	fmt::memory_buffer formatBuf;
	LogLevel logLevel;
};

CORE_API void InitializeLogger(LogLevel level);
CORE_API void DeinitializeLogger();
CORE_API Logger& GetLogger();


#define MUSA_LOG(LogSlot, level, msg, ...) GetLogger().Log(level, msg, ##__VA_ARGS__)

#define MUSA_DEBUG(LogSlot, msg, ...) MUSA_LOG(LogSlot, LogLevel::Debug, msg, ##__VA_ARGS__)
#define MUSA_INFO(LogSlot, msg, ...) MUSA_LOG(LogSlot, LogLevel::Info, msg, ##__VA_ARGS__)
#define MUSA_WARN(LogSlot, msg, ...) MUSA_LOG(LogSlot, LogLevel::Warning, msg, ##__VA_ARGS__)
#define MUSA_ERR(LogSlot, msg, ...) MUSA_LOG(LogSlot, LogLevel::Error, msg, ##__VA_ARGS__)
#define MUSA_FATAL(LogSlot, msg, ...) MUSA_LOG(LogSlot, LogLevel::Fatal, msg, ##__VA_ARGS__)




