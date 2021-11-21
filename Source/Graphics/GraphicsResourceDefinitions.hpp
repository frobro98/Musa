// Copyright 2020, Nathan Blane

#pragma once

#include "Debugging/Assertion.hpp"

// Textures
struct NativeTexture
{ 
protected:
	~NativeTexture() {}
};
struct NativeSampler
{
protected:
	~NativeSampler() {}
};

// Buffers
struct NativeUniformBuffer
{
protected:
	~NativeUniformBuffer() {}
};

struct NativeStorageBuffer
{
protected:
	~NativeStorageBuffer() {}
};

struct NativeVertexBuffer
{
protected:
	~NativeVertexBuffer() {}
};

struct NativeIndexBuffer
{ 
protected:
	~NativeIndexBuffer() {}
};

// Viewport
struct NativeViewport
{
protected:
	~NativeViewport() {}
};

// Shaders
struct NativeVertexShader
{
protected:
	~NativeVertexShader() {}
};

struct NativeGeometryShader
{
protected:
	~NativeGeometryShader() {}
};

struct NativeTessEvaluationShader 
{
protected:
	~NativeTessEvaluationShader() {}
};

struct NativeTessControlShader 
{
protected:
	~NativeTessControlShader() {}
};

struct NativeFragmentShader 
{
protected:
	~NativeFragmentShader() {}
};

struct NativeComputeShader 
{
protected:
	~NativeComputeShader() {}
};

