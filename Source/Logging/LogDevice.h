#pragma once

#include "Platform.h"

class LogDevice
{
public:
	LogDevice();
	virtual ~LogDevice() = default;

	virtual void Log(const tchar* logStr) = 0;
};