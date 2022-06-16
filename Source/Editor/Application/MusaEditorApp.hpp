#pragma once


#include "Musa/Application/MusaApp.hpp"

// TODO - Move this somewhere else?
struct ViewSizing
{
	u32 width;
	u32 height;

	u32 requestedWidth;
	u32 requestedHeight;
};

class MusaEditorApp final : public MusaApp
{
public:
	virtual void AppInit() override;
	virtual void AppLoop(f32 tick) override;
	virtual void AppDeinit() override;

private:
	ViewSizing viewSize{};
};