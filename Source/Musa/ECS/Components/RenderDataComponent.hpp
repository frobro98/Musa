// Copyright 2020, Nathan Blane

#pragma once

#include "ECS/Component.hpp"
#include "BasicTypes/UniquePtr.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Shader/MaterialRenderDescription.hpp"

struct RenderDataComponent : Musa::Component
{
	UniquePtr<NativeUniformBuffer> primBuffer;
	MaterialRenderDescription materialDesc;
};

