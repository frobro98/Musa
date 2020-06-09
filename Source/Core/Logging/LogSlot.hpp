#pragma once

struct CORE_API LogSlot
{
	LogSlot(const tchar* name) : logName(name) {}

	// TODO - Need to move to a compile time/"Name" system for strings
	StringView logName;
	// TODO - Figure out if I need anything else to allow for log filtering down the line

};

#define DEFINE_LOG_SLOT(SystemName)									\
	inline struct SystemName##LogSlot : LogSlot			\
	{															\
		SystemName##LogSlot() : LogSlot(#SystemName) {}	\
	} SystemName

