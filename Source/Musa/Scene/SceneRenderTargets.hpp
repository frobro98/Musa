// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "Graphics/RenderTarget.hpp"

struct SceneRenderTargets
{
	UniquePtr<RenderTarget> sceneColorTexture;
	UniquePtr<RenderTarget> depthTexture;
};

