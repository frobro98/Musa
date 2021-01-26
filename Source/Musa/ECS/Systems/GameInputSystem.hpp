// Copyright 2020, Nathan Blane

#pragma once

#include "ECS/System.hpp"

class GameInputSystem : Musa::System
{
public:
	virtual void Initialize() override;
	virtual void Update() override;

private:
	void ProcessInputEvents() const;
	void ClearInputEvents();

private:
	Musa::Query* gameInputInitQuery = nullptr;
	Musa::Query* existingGameInputQuery = nullptr;
	Musa::Query* buttonEventQuery = nullptr;
	Musa::Query* mouseMoveEventQuery = nullptr;
	Musa::Query* analogEventQuery = nullptr;
};
