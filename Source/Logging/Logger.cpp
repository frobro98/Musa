
#include "Logger.h"
#include "LogDevice.h"
#include "String/CStringUtilities.hpp"

constexpr const uint32 LogBufferLength = 512;

void Logger::AddLogDevice(LogDevice * logger)
{
	Instance().loggingDevices.Add(logger);
}

void Logger::RemoveLogDevice(LogDevice* /*logger*/)
{
	// TODO - Actually add a way to remove from an array....
// 	for(uint32 i = 0; i < Instance().loggingDevices.Size())
// 	Instance().loggingDevices.Remove()
}

void Logger::Log(const tchar* format, ...)
{
	// TODO - Need to add a newline after every log entry
	tchar bufferedLog[LogBufferLength];
	VA_ARGS_PARSE(format, bufferedLog, LogBufferLength);
	Instance().LogFormated(bufferedLog);
}

Logger::~Logger()
{
	for (uint32 i = 0; i < loggingDevices.Size(); ++i)
	{
		delete loggingDevices[i];
	}

	loggingDevices.Clear();
}

Logger & Logger::Instance()
{
	static Logger lInstance;
	return lInstance;
}

void Logger::LogFormated(const tchar * str)
{
	assert(loggingDevices.Size() > 0);
	for (uint32 i = 0; i < loggingDevices.Size(); ++i)
	{
		// Push logging to other devices
		loggingDevices[i]->Log(str);
	}
}
