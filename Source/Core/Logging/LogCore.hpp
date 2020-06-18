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

// TODO - Not really cleaning things up currently. If I delete the thread, then the os thread execution
// will be using dealloced memory. Need a better way of cleaning things up potentially
class CORE_API Logger
{
public:
	template <typename... Args>
	forceinline void Log(const LogChannel& logChannel, LogLevel::Type level, const tchar* msg, Args&&... args)
	{
		if (ShouldLogAtLevel(level))
		{
			fmt::format_to(formatBuf, msg, args...);
			PushLineToLog(logChannel, logLevel, formatBuf.data(), formatBuf.size());
			formatBuf.clear();
		}
	}

	forceinline void Log(LogLevel::Type level, const tchar* msg)
	{
		Log(DefaultLog, level, msg);
	}

	forceinline void Log(const LogChannel& logChannel, LogLevel::Type level, const tchar* msg)
	{
		if (ShouldLogAtLevel(level))
		{
			PushLineToLog(logChannel, logLevel, msg, Strlen(msg));
		}
	}

	forceinline bool ShouldLogAtLevel(LogLevel::Type level)
	{
		return level <= logLevel;
	}

	forceinline void SetLogLevel(LogLevel::Type level)
	{
		logLevel = level;
	}
	forceinline LogLevel::Type GetLogLevel() const
	{
		return logLevel;
	}

	void AddLogSink(LogSink& sink);
	void RemoveLogSink(LogSink& sink);

private:
	void PushLineToLog(const LogChannel& logChannel, LogLevel::Type level, const tchar* msg, size_t msgSize);

	friend CORE_API void InitializeLogger(LogLevel::Type level);
private:
	// Threading!
	LoggingThread* loggingThread = nullptr;
	fmt::memory_buffer formatBuf;
	LogLevel::Type logLevel;
};

CORE_API void InitializeLogger(LogLevel::Type level);
CORE_API void DeinitializeLogger();
CORE_API Logger& GetLogger();


#define MUSA_LOG(Channel, level, msg, ...) GetLogger().Log(Channel, level, msg, ##__VA_ARGS__)

#define MUSA_DEBUG(Channel, msg, ...) MUSA_LOG(Channel, LogLevel::Debug, msg, ##__VA_ARGS__)
#define MUSA_INFO(Channel, msg, ...) MUSA_LOG(Channel, LogLevel::Info, msg, ##__VA_ARGS__)
#define MUSA_WARN(Channel, msg, ...) MUSA_LOG(Channel, LogLevel::Warning, msg, ##__VA_ARGS__)
#define MUSA_ERR(Channel, msg, ...) MUSA_LOG(Channel, LogLevel::Error, msg, ##__VA_ARGS__)
#define MUSA_FATAL(Channel, msg, ...) MUSA_LOG(Channel, LogLevel::Fatal, msg, ##__VA_ARGS__)




