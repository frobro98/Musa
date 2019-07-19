#pragma once

#include "Logging/LogDevice.h"

class DebugWindowDevice final : public LogDevice
{
public:
	DebugWindowDevice() = default;
	~DebugWindowDevice() {}

	virtual void Log(const tchar* logStr) override;
};