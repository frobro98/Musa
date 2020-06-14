#pragma once

#include "Platform/Platform.hpp"
#include "Threading/ISyncEvent.hpp"

class Win32SyncEvent : public ISyncEvent
{
public:
	Win32SyncEvent() = default;
	~Win32SyncEvent();

	virtual void Create(bool manualReset /* = false */) override;
	virtual bool Wait(u32 waitTime /* = U32Max */) override;
	virtual void Set() override;
	virtual bool Reset() override;
	virtual bool IsManualReset() const override;
	virtual bool IsValid() const override;

private:
	HANDLE eventHandle = nullptr;
	bool isManualReset = false;
};
