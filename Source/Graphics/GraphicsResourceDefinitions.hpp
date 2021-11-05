// Copyright 2020, Nathan Blane

#pragma once

#include "Debugging/Assertion.hpp"

struct NativeTexture { ~NativeTexture() { Assertf(false, "Deleting a NativeTexture pointer! Release resource by calling GraphicsInterface::DestroyTexture"); } };
struct NativeSampler { ~NativeSampler() { Assertf(false, "Deleting a NativeSampler pointer!"); } };

struct NativeUniformBuffer { virtual ~NativeUniformBuffer() { Assertf(false, "Deleting a NativeUniformBuffer pointer! Release resource by calling GraphicsInterface::DestroyUniformBuffer"); } };
struct NativeStorageBuffer { virtual ~NativeStorageBuffer() { Assertf(false, "Deleting a NativeStorageBuffer pointer! Release resource by calling GraphicsInterface::DestroyStorageBuffer"); } };
struct NativeVertexBuffer { virtual ~NativeVertexBuffer() { Assertf(false, "Deleting a NativeVertexBuffer pointer! Release resource by calling GraphicsInterface::DestroyVertexBuffer"); } };
struct NativeIndexBuffer { virtual ~NativeIndexBuffer() { Assertf(false, "Deleting a NativeIndexBuffer pointer! Release resource by calling GraphicsInterface::DestroyIndexBuffer"); } };

struct NativeViewport { virtual ~NativeViewport() { Assertf(false, "Deleting a NativeViewport pointer! Release resource by calling GraphicsInterface::DestroyViewport"); } };

struct NativeVertexShader { virtual ~NativeVertexShader() { Assertf(false, "Deleting a NativeVertexShader pointer! Release resource by calling GraphicsInterface::DestroyVertexShader"); } };
struct NativeGeometryShader { virtual ~NativeGeometryShader() { Assertf(false, "Deleting a NativeGeometryShader pointer! Release resource by calling GraphicsInterface::DestroyGeometryShader"); }};
struct NativeTessEvaluationShader { virtual ~NativeTessEvaluationShader() { Assertf(false, "Deleting a NativeTessEvaluationShader pointer! Release resource by calling GraphicsInterface::DestroyTessEvaluationShader"); } };
struct NativeTessControlShader { virtual ~NativeTessControlShader() { Assertf(false, "Deleting a NativeTessControlShader pointer! Release resource by calling GraphicsInterface::DestroyTessControlShader"); } };
struct NativeFragmentShader { virtual ~NativeFragmentShader() { Assertf(false, "Deleting a NativeFragmentShader pointer! Release resource by calling GraphicsInterface::DestroyFragmentShader"); } };
struct NativeComputeShader { virtual ~NativeComputeShader() { Assertf(false, "Deleting a NativeComputeShader pointer! Release resource by calling GraphicsInterface::DestroyComputeShader"); } };

