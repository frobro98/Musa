#include "MusaAppOS.hpp"

MusaAppOS::MusaAppOS(UniquePtr<WindowInputHandler>&& inputHandler_)
	: inputHandler(std::move(inputHandler_))
{
}
