#pragma once

#include "Threading/IThreadBody.hpp"
#include "Threading/NativeThread.hpp"

// Should own all structures that contain their portions to write to. 
// This should be all of the log sinks are put here instead of the logger. 
// Logger just funnels messages to this logging body

class LoggingThread : IThreadBody
{
public:
	virtual void ThreadBody() override;

private:
	NativeThread* logThread;
};
