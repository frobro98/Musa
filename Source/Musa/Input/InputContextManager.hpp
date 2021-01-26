// Copyright 2020, Nathan Blane

#pragma once

#include "Containers/Map.h"
#include "String/StringView.hpp"
#include "Input/InputContextID.hpp"

struct InputContext;

// NOTE - This exists purely for managing the eventual input context files. These will represent multiple different
// game input contexts
class InputContextManager
{
public:
	InputContextID AddInputContext(StringView contextName, InputContext& context);
	void RemoveInputContext(InputContextID id);

	InputContext& FindLoadedInputContext(InputContextID id);
	InputContextID NameToID(StringView name) const;

private:
	Map<u32, InputContext*> contextsByID;
};

InputContextManager& GetInputContextManager();

