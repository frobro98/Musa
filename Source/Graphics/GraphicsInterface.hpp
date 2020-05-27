// Copyright 2020, Nathan Blane

#pragma once

#include "GraphicsResourceFlags.hpp"
#include "BasicTypes/UniquePtr.hpp"
#include "Texture/ImageFormats.h"
#include "RenderTargetDescription.hpp"
#include "Containers/DynamicArray.hpp"
#include "Containers/MemoryBuffer.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Graphics/GraphicsAPI.hpp"

struct Vertex;
struct Face;
class ResourceBlob;
class RenderContext;
struct SamplerDescription;

class GRAPHICS_API GraphicsInterface
{
public:

	// TODO - Decide on whether or not to put the Begin/EndFrame calls in this interface or a different interface more focused on the actual render pipeline

	virtual ~GraphicsInterface() = default;

	virtual void InitializeGraphics() = 0;
	virtual void DeinitializeGraphics() = 0;
	
	NODISCARD virtual UniquePtr<NativeVertexShader> CreateVertexShader(const MemoryBuffer& vertexCode) = 0;
	NODISCARD virtual UniquePtr<NativeFragmentShader> CreateFragmentShader(const MemoryBuffer& fragmentCode) = 0;
	NODISCARD virtual UniquePtr<NativeGeometryShader> CreateGeometryShader(const MemoryBuffer& geometryCode) = 0;
	NODISCARD virtual UniquePtr<NativeTessEvaluationShader> CreateTessEvaluationShader(const MemoryBuffer& tessEvalCode) = 0;
	NODISCARD virtual UniquePtr<NativeTessControlShader> CreateTessControlShader(const MemoryBuffer& tessCtrlCode) = 0;
	NODISCARD virtual UniquePtr<NativeComputeShader> CreateComputeShader(const MemoryBuffer& computeCode) = 0;

	NODISCARD virtual UniquePtr<NativeViewport> CreateViewport(void* windowHandle, u32 viewWidth, u32 viewHeight) = 0;

	NODISCARD virtual UniquePtr<NativeVertexBuffer> CreateVertexBuffer(const DynamicArray<Vertex>& vertices) const = 0;
	NODISCARD virtual UniquePtr<NativeIndexBuffer> CreateIndexBuffer(const DynamicArray<Face>& faces) const = 0;
	NODISCARD virtual UniquePtr<NativeUniformBuffer> CreateUniformBuffer(u32 bufferSize) const = 0;
	// TODO - Consider the model where the mapped ptr is returned and then the user does with it what they wish
	virtual void PushBufferData(NativeUniformBuffer& buffer, const void* data) const = 0;

	NODISCARD virtual UniquePtr<NativeTexture> CreateEmptyTexture2D(u32 width, u32 height, ImageFormat textureFormat, u32 mipLevels, TextureUsage::Type usage) = 0;
	NODISCARD virtual UniquePtr<NativeTexture> CreateInitializedTexture2D(const ResourceBlob& textureBlob, u32 width, u32 height, ImageFormat textureFormat, u32 mipLevels, TextureUsage::Type usage) = 0;
	NODISCARD virtual NativeSampler* CreateTextureSampler(const SamplerDescription& params) = 0;
	virtual void PushTextureData(NativeTexture& texture, const ResourceBlob& textureBlob) = 0;

	NODISCARD virtual void* GetGraphicsDevice() = 0;
	NODISCARD virtual RenderContext* GetRenderContext() = 0;
};

GRAPHICS_API GraphicsInterface& GetGraphicsInterface();
