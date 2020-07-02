#include "LogFileSink.hpp"
#include "Logging/LogLineEntry.hpp"

// TODO - Initial time stamp of file open, down to the millisecond
// Then after the time stamp, it uses the offset from that in milliseconds
static void FormatDateAndTime(fmt::memory_buffer& buff)
{
	// TODO - Windows Specific!!!
	SYSTEMTIME systemTime;
	// GetLocalTime gets the time with the time zone applied. 
	// GetSystemTime gets the time with the time zone NOT applied (UTC)
	::GetLocalTime(&systemTime);

	fmt::format_to(buff, "Musa log opened at {:02d}:{:02d}:{:02d}.{:03d} on {:02d}/{:02d}/{:04d}\n",
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds,
		systemTime.wMonth, systemTime.wDay, systemTime.wYear);
}

static fmt::memory_buffer logLineEntryBuffer;

LogFileSink::LogFileSink(const Path& filePath)
{
	File::Result result = File::Open(logFileHandle, filePath.GetString(), File::Mode::WRITE);
	Assert(result == File::Result::SUCCESS);

	fmt::memory_buffer logHeader;
	FormatDateAndTime(logHeader);
	File::Write(logFileHandle, logHeader.data(), (u32)logHeader.size());
}

LogFileSink::~LogFileSink()
{
	const tchar* endLogText = "Log closed, application terminated\n";
	File::Result result = File::Write(logFileHandle, endLogText, (u32)Strlen(endLogText));
	Assert(result == File::Result::SUCCESS);

	// TODO - Say when we closed it, just so we know when the thread terminated...
	result = File::Close(logFileHandle);
	Assert(result == File::Result::SUCCESS);
}

void LogFileSink::OutputFormattedString(const LogLineEntry& entry)
{
	// TODO - Get time difference from the start of the program execution
		// Format goes: Time since start up, log level, log channel, log msg
	fmt::format_to(logLineEntryBuffer, "[TODO - TimeSinceBegin][{:s}]({:.{}}):{:s}\n",
		ToString(entry.level), *entry.logSlot, entry.logSlot.Length(), *entry.logMsg);

	NOT_USED File::Result result = File::Write(logFileHandle, logLineEntryBuffer.data(), (u32)logLineEntryBuffer.size());
	Assert(result == File::Result::SUCCESS);

	logLineEntryBuffer.clear();
}