
#include "Input.hpp"
#include "Input/InputManager.h"


class InputMan
{
public:
	void Initialize(Window& /*win*/)
	{

	}

	void AddContext(const InputContext& context)
	{
		contexts.Add(context);
	}

	void PushActiveContext(StringView contextName)
	{
		InputContext* context = contexts.FindFirst([&contextName](const InputContext& c) { return c.contextName == contextName; });
		if (context)
		{
			activeContexts.Add(context);
		}
	}

	void PopActiveContext(StringView contextName)
	{
		InputContext** context = activeContexts.FindFirst([&contextName](const InputContext* c) { return c->contextName == contextName; });
		if (context)
		{
			activeContexts.RemoveAll(*context);
		}
	}

	void RegisterCallback(InputCallback&& callback)
	{
		callbacks.Add(std::move(callback));
	}



	// TODO - Figure out if we want to actually get rid of a callback...
	//void UnregisterCallback(InputCallback)

private:
	// TODO - These should probably be sets not arrays
	DynamicArray<InputContext> contexts;
	DynamicArray<InputCallback> callbacks;
	DynamicArray<InputContext*> activeContexts;
};

namespace
{
InputMan inputManager;
}

InputContext MakeContext(const String& name)
{
	InputContext context = {};
	context.contextName = name;
	context.nameHash = GetHash(name);
	return context;
}

void InitializeInput(Window& win)
{
	inputManager.Initialize(win);
}

void AddCallback(InputCallback&& callback)
{
	inputManager.RegisterCallback(std::move(callback));
}


void AddInputContext(const InputContext& context)
{
	inputManager.AddContext(context);
}

void PushInputContext(StringView contextName)
{
	inputManager.PushActiveContext(contextName);
}

void RemoveInputContext(StringView contextName)
{
	inputManager.PopActiveContext(contextName);
}

bool IsInputPressed(Inputs::Type /*key*/)
{
	return false;
}

Vector2 GetMousePosition()
{
	return Vector2();
}

InputManager& GetInputManager()
{
	return InputManager::Instance();
}