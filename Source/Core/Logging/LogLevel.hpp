#pragma once

enum class LogLevel
{
	Debug,
	Info,
	Warning,
	Error,
	Fatal
};

forceinline const tchar* ToString(LogLevel level)
{
	// TODO - Implement enum strings with macro tricks?
	switch (level)
	{
		case LogLevel::Debug: return "DBG";
		case LogLevel::Info: return "INF";
		case LogLevel::Warning: return "WRN";
		case LogLevel::Error: return "ERR";
		case LogLevel::Fatal: return "FATAL";
		default:
			return nullptr;
	}
}
