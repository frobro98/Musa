// Copyright 2020, Nathan Blane

#pragma once

#include "Application/MusaApp.hpp"
#include "Engine/MusaEngine.hpp"
#include "Game/GameAPI.hpp"

class GAME_API MusaGameApp final : public MusaApp
{
public:
	virtual void AppInit() override;
	virtual void AppLoop(f32 tick, const DynamicArray<Input::Event>& frameInputs) override;
	virtual void AppDeinit() override;

private:
	UniquePtr<MusaEngine> gameEngine;
};