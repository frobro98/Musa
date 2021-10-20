// Copyright 2020, Nathan Blane

#pragma once

#include "Application/MusaApp.hpp"

class MUSA_API MusaGameApp final : public MusaApp
{
public:
	virtual void AppInit() override;
	virtual void AppLoop() override;
	virtual void AppDeinit() override;

private:
	EngineTick frameTick;

};