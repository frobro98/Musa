#pragma once

#include "Logging/LogSink.hpp"
#include "File/FileSys.hpp"

// TODO - This currently doesn't make sense to use outside of the logger
// This doesn't really offer much except logging the entire log to a file. 
// This doesn't really offer much in terms of usefulness outside of the logging system
// so I feel like it should be able to be used outside (i.e. open a file and log specific slots to it)
class CORE_API LogFileSink final : public LogSink
{
public:
	LogFileSink(const Path& filePath);
	~LogFileSink();

	virtual void OutputFormattedString(const LogLineEntry& entry) override;

private:
	File::Handle logFileHandle;
};
