#pragma once

struct NativeTexture { virtual ~NativeTexture() = default; };
struct NativeSampler { virtual ~NativeSampler() = default; };

struct NativeUniformBuffer { virtual ~NativeUniformBuffer() = default; };
struct NativeStorageBuffer { virtual ~NativeStorageBuffer() = default; };
struct NativeVertexBuffer { virtual ~NativeVertexBuffer() = default; };
struct NativeIndexBuffer { virtual ~NativeIndexBuffer() = default; };

struct NativeViewport { virtual ~NativeViewport() = default; };

struct NativeVertexShader { virtual ~NativeVertexShader() = default; };
struct NativeGeometryShader { virtual ~NativeGeometryShader() = default; };
struct NativeTessEvaluationShader { virtual ~NativeTessEvaluationShader() = default; };
struct NativeTessControlShader { virtual ~NativeTessControlShader() = default; };
struct NativeFragmentShader { virtual ~NativeFragmentShader() = default; };
struct NativeComputeShader { virtual ~NativeComputeShader() = default; };

