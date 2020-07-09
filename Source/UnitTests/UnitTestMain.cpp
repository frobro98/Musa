// Copyright 2020, Nathan Blane

#include "Framework/UnitTest.h"
#include "Logging/LogCore.hpp"

int main()
{
	//InitializeLogger(LogLevel::Info);

	UnitTestRegistry::RunTests();

	return 0;
}