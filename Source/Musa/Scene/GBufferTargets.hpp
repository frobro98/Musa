// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "Graphics/RenderTarget.hpp"

struct GBuffer
{
	UniquePtr<RenderTarget> positionTexture;
	UniquePtr<RenderTarget> normalTexture;
	UniquePtr<RenderTarget> diffuseTexture;
};


