#pragma once

#include "Types/UniquePtr.hpp"
#include "Graphics/RenderTarget.hpp"

struct GBuffer
{
	UniquePtr<RenderTarget> positionTexture;
	UniquePtr<RenderTarget> normalTexture;
	UniquePtr<RenderTarget> diffuseTexture;
};


