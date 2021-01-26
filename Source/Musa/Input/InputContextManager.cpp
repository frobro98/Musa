// Copyright 2020, Nathan Blane

#include "InputContextManager.hpp"
#include "InputContext.hpp"

InputContextID InputContextManager::AddInputContext(StringView /*contextName*/, InputContext& context)
{
	InputContextID id = context.id;
	
	return id;
}

void InputContextManager::RemoveInputContext(InputContextID id)
{
	contextsByID.Remove(id.hash);
}

InputContext& InputContextManager::FindLoadedInputContext(InputContextID id)
{
	InputContext** context = contextsByID.Find(id.hash);
	Assert(context);

	return **context;
}

InputContextID InputContextManager::NameToID(StringView name) const
{
	return InputContextID{ GetHash(name) };
}

InputContextManager& GetInputContextManager()
{
	static InputContextManager icMan;
	return icMan;
}
