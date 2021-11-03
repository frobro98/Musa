#pragma once


#include "Musa/Application/MusaApp.hpp"

class MusaEditorApp final : public MusaApp
{
public:
	virtual void AppInit() override;
	virtual void AppLoop(f32 tick, const DynamicArray<Input::Event>& frameInputs) override;
	virtual void AppDeinit() override;
};