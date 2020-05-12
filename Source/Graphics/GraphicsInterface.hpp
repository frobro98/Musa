// Copyright 2020, Nathan Blane

#pragma once

#include "GraphicsResourceFlags.hpp"
#include "Types/UniquePtr.hpp"
#include "Texture/ImageFormats.h"
#include "RenderTargetDescription.hpp"
#include "Containers/DynamicArray.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

struct Vertex;
struct Face;
class ResourceBlob;
class RenderContext;
struct SamplerDescription;

class GraphicsInterface
{
public:

	// TODO - Decide on whether or not to put the Begin/EndFrame calls in this interface or a different interface more focused on the actual render pipeline

	virtual ~GraphicsInterface() = default;

	virtual void InitializeGraphics() = 0;
	virtual void DeinitializeGraphics() = 0;
	
	NODISCARD virtual UniquePtr<NativeViewport> CreateViewport(void* windowHandle, uint32 viewWidth, uint32 viewHeight) = 0;

	NODISCARD virtual UniquePtr<NativeVertexBuffer> CreateVertexBuffer(const DynamicArray<Vertex>& vertices) const = 0;
	NODISCARD virtual UniquePtr<NativeIndexBuffer> CreateIndexBuffer(const DynamicArray<Face>& faces) const = 0;
	NODISCARD virtual UniquePtr<NativeUniformBuffer> CreateUniformBuffer(uint32 bufferSize) const = 0;
	// TODO - Consider the model where the mapped ptr is returned and then the user does with it what they wish
	virtual void PushBufferData(NativeUniformBuffer& buffer, const void* data) const = 0;

	NODISCARD virtual UniquePtr<NativeTexture> CreateEmptyTexture2D(uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) = 0;
	NODISCARD virtual UniquePtr<NativeTexture> CreateInitializedTexture2D(const ResourceBlob& textureBlob, uint32 width, uint32 height, ImageFormat textureFormat, uint32 mipLevels, TextureUsage::Type usage) = 0;
	NODISCARD virtual NativeSampler* CreateTextureSampler(const SamplerDescription& params) = 0;
	virtual void PushTextureData(NativeTexture& texture, const ResourceBlob& textureBlob) = 0;

	NODISCARD virtual void* GetGraphicsDevice() = 0;
	NODISCARD virtual RenderContext* GetRenderContext() = 0;
};

GraphicsInterface& GetGraphicsInterface();
