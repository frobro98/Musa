#include "LogFileSink.hpp"
#include "File/FileSys.hpp"

LogFileSink::LogFileSink()
{

}

LogFileSink::~LogFileSink()
{
}

void LogFileSink::OutputFormattedString(const LogLineEntry& entry)
{
	UNUSED(entry);
}
