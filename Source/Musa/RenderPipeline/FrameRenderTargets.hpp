#pragma once

#include "Types/UniquePtr.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

struct GBuffer
{
	UniquePtr<NativeTexture> positionTexture;
	UniquePtr<NativeTexture> normalTexture;
	UniquePtr<NativeTexture> diffuseTexture;
};

struct SceneRenderTargets
{
	UniquePtr<NativeTexture> sceneColorTexture;
	UniquePtr<NativeTexture> depthTexture;
};

struct FrameRenderTargets
{
	GBuffer gbuffer;
	SceneRenderTargets sceneTargets;
	UniquePtr<NativeTexture> userInterfaceTarget;
};