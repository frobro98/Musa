#pragma once

#include "PlatformDefinitions.h"
#include "InputDefinitions.hpp"

class Window;
class GameObject;

namespace Internal
{
void KeyMessageDownReceived(uint32 keyCode, bool isPressed, bool isRepeated);
void KeyMessageUpReceived(uint32 keyCode);
void MouseMovementChange();
}

class InputManager final
{
public:
	void Initialize(Window& win);
	
	// TODO - Should probably change this to GameObject instead of template class C
	template <class C, typename Func>
	void RegisterContinuousInput(KeyInput key, int32 stateModifier, C* object, Func func)
	{
		static_assert(std::is_base_of_v<GameObject, C>, "Only GameObjects are allowed to be registered for input");
		static_assert(std::is_member_function_pointer_v<Func>, "Function passed in must be a member of C");
		static_assert(std::is_class_v<C>, "C must be a class");
		
		SetContinuousInputFunction(key, stateModifier, std::bind(func, object, std::placeholders::_1));
	}

	// TODO - rename this to something that represents press and release events
	template <class C, typename Func>
	void RegisterStateChange(KeyInput key, KeyStateFlags keyState, C* object, Func func)
	{
		static_assert(std::is_base_of_v<GameObject, C>, "Only GameObjects are allowed to be registered for input");
		static_assert(std::is_member_function_pointer_v<Func>, "Function passed in must be a member of C");
		static_assert(std::is_class_v<C>, "C must be a class");

		SetStateInputFunction(key, keyState, std::bind(func, object));
	}

	template<class C, typename Func>
	void RegisterMouseAxes(C* object, Func func)
	{
		static_assert(std::is_base_of_v<GameObject, C>, "Only GameObjects are allowed to be registered for input");
		static_assert(std::is_member_function_pointer_v<Func>, "Function passed in must be a member of C");
		static_assert(std::is_class_v<C>, "C must be a class");

		SetMouseAxesFunction(std::bind(func, object, std::placeholders::_1, std::placeholders::_2));
	}

	// TODO - Consider moving this into a more Input specialized class instead of in a manager class
	bool IsKeyPressed(KeyInput key) const;

	void Update();

	static InputManager& Instance();

private:
	using ContinuousInputFunction = std::function<void(int)>;
	using StateChangedInputFunction = std::function<void()>;
	using MousePositionFunction = std::function<void(float, float)>;

	void SetContinuousInputFunction(KeyInput key, int32 stateMod, ContinuousInputFunction&& inputFunc);
	void SetStateInputFunction(KeyInput key, KeyStateFlags state, StateChangedInputFunction&& inputFunc);
	void SetMouseAxesFunction(MousePositionFunction&& mouseFunction);
	void ProcessInputMessages();
	void ProcessMouseMovement();
	void BroadcastInputMessages();

	InputManager() = default;
	~InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager& operator=(InputManager&&) = delete;

private:
	Window* window;

	bool shiftMod;
	bool controlMod;
	bool altMod;
	bool pad;
};
