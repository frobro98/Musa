// Copyright 2020, Nathan Blane

#pragma once

#include "Application/MusaApp.hpp"

class MusaGameApp final : public MusaApp
{
public:
	virtual void AppInit() override;
	virtual void AppLoop() override;
	virtual void AppDeinit() override;

private:
	EngineTick frameTick;

};