#pragma once

#include "Containers/Array.h"

class LogDevice;

// TODO - Actually make this a multi-threaded system. This is pretty dumb at this point...
class Logger final
{
public:
	static void AddLogDevice(LogDevice* logger);
	static void RemoveLogDevice(LogDevice* logger);

	static void Log(const tchar* format, ...);

private:
	Logger() = default;
	~Logger();

	static Logger& Instance();

	void LogFormated(const tchar* str);
private:
	Array<LogDevice*> loggingDevices;

};