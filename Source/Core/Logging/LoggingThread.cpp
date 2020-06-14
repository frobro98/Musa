#include "LoggingThread.hpp"
#include "Platform/PlatformThreading.hpp"
#include "File/DirectoryLocations.hpp"
#include "Threading/ISyncEvent.hpp"

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



LoggingThread::LoggingThread(DynamicArray<LogSink*>&& sinks)
	: logOutputSinks(std::move(sinks))
{
	linePushedEvent = PlatformThreading::CreateSyncEvent(false);
	// NOTE - These calls shouldn't be combined
	logThread = PlatformThreading::CreateThread();
	logThread->StartWithBody("Log Thread", ThreadPriority::High, *this);
}

LoggingThread::~LoggingThread()
{
	linePushedEvent->Set();
	logThread->WaitStop();
}

void LoggingThread::AddLogSink(LogSink& sink)
{
	REF_CHECK(sink);
	ScopedLock sinkLock(sinksCriticalSection);
	logOutputSinks.Add(&sink);
}

void LoggingThread::RemoveSink(LogSink& sink)
{
	REF_CHECK(sink);
	ScopedLock sinkLock(sinksCriticalSection);
	logOutputSinks.RemoveFirstOf(&sink);
}

void LoggingThread::PushLogLine(const LogLineEntry& logLine)
{
	{
		ScopedLock logEntryLock(unprocessedCriticalSection);
		entriesToProcess.Add(logLine);
	}
	linePushedEvent->Set();
}

void LoggingThread::ThreadInit()
{
	Assert(linePushedEvent);
	Assert(logThread);

	Path logFile = Path(EngineLogPath()) / "musa.log";
	File::Result result = File::Open(logFileHandle, logFile.GetString(), File::Mode::WRITE);
	Assert(result == File::Result::SUCCESS);
	
	fmt::memory_buffer logHeader;
	FormatDateAndTime(logHeader);
	File::Write(logFileHandle, logHeader.data(), (u32)logHeader.size());
}

void LoggingThread::ThreadBody()
{
	fmt::memory_buffer logLineEntryBuffer;

	bool shouldWait = true;
	while (!stopRequested)
	{
		if (shouldWait)
		{
			linePushedEvent->Wait();
			shouldWait = false;
		}

		LogLineEntry entry;
		{
			ScopedLock logEntryLock(unprocessedCriticalSection);
			entry = entriesToProcess.Last();
			entriesToProcess.RemoveLast();
			shouldWait = entriesToProcess.IsEmpty();
		}

		// TODO - Get time difference from the start of the program execution
		// Format goes: Time since start up, log level, log channel, log msg
		fmt::format_to(logLineEntryBuffer, "[TODO - TimeSinceBegin][{:s}]({:.{}}):{:s}\n",
			ToString(entry.level), *entry.logSlot, entry.logSlot.Length(), *entry.logMsg);

		File::Result result = File::Write(logFileHandle, logLineEntryBuffer.data(), (u32)logLineEntryBuffer.size());
		if (result != File::Result::SUCCESS)
		{
			File::Close(logFileHandle);
			break;
		}

		// Process entry
		//logLineEntries.Add(entry);
	}
}

void LoggingThread::ThreadExit()
{
	Assert(stopRequested);
	// TODO - Say when we closed it, just so we know when the thread terminated...
	File::Close(logFileHandle);
}

void LoggingThread::RequestStop()
{
	// TODO - This needs to be an atomic operation...
	stopRequested = true;
}
