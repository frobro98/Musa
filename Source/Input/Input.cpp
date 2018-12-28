
#include "Input.hpp"
#include "Input/InputManager.h"

InputManager& GetInputManager()
{
	return InputManager::Instance();
}