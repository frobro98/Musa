#pragma once


#include "Musa/Application/MusaApp.hpp"

class MusaEditorApp final : public MusaApp
{
public:
	virtual void AppInit() override;
	virtual void AppLoop() override;
	virtual void AppDeinit() override;
};