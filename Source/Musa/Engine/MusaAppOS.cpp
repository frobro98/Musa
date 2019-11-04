#include "MusaAppOS.hpp"

MusaAppOS::MusaAppOS(UniquePtr<WindowInputHandler>&& inputHandler_)
	: inputHandler(std::move(inputHandler_))
{
}

void MusaAppOS::PostProcessInputEvents()
{
	inputHandler->PostUpdateInput();
}
