#pragma once

#include "String/String.h"
#include "Logging/LogSlot.hpp"
#include "Logging/LogLevel.hpp"

// TODO - This sort of represents a line within the log. This will be changed to 
// show this once logging works
struct LogEntry
{
	String logMsg;
	String logSlot;
	LogLevel level;
};
