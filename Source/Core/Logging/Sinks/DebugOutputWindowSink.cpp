#include "DebugOutputWindowSink.hpp"
#include "Logging/LogLineEntry.hpp"

static fmt::memory_buffer logLineEntryBuffer;

void DebugOutputWindowSink::OutputFormattedString(const LogLineEntry& entry)
{
	constexpr size_t outputLen = 512;
	tchar outputStr[outputLen] = {};

	fmt::format_to(logLineEntryBuffer, "[TODO - TimeSinceBegin][{:s}]({:.{}}):{:s}\n",
		ToString(entry.level), *entry.logSlot, entry.logSlot.Length(), *entry.logMsg);

	size_t len = logLineEntryBuffer.size() >= outputLen ? outputLen - 1 : logLineEntryBuffer.size();
	Memory::Memcpy(outputStr, logLineEntryBuffer.data(), len);

	OutputDebugString(outputStr);

	logLineEntryBuffer.clear();
}