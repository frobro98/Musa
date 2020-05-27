// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"

// TODO - move this enum to a better place
enum class LoadOperation : u8
{
	Load,
	Clear,
	DontCare
};

// TODO - move this enum to a better place
enum class StoreOperation : u8
{
	Store,
	DontCare
};

enum class CompareOperation : u8
{
	None,
	Equal,
	NotEqual,
	Less,
	LessThanOrEqual,
	Greater,
	GreaterThanOrEqual,
	Always
};

enum class StencilOperation : u8
{
	Keep,
	Zero,
	Replace,
	Invert,
	IncrementAndClamp,
	DecrementAndClamp,
	IncrementAndWrap,
	DecrementAndWrap
};

enum class PrimitiveTopology : u32
{
	TriangleList,
	TriangleStrip,
	TriangleFan,
	LineList,
	LineStrip,
	PointList
};

enum class CullingMode : u32
{
	None,
	Back,
	Front,
	FrontAndBack
};

enum class ShadingModel : u32
{
	Lit,
	Unlit
};

enum class FillMode : u32
{
	Full,
	Wireframe,
	Point
};

enum class ColorMask : u8
{
	Red = 0x1,
	Green = 0x2,
	Blue = 0x4,
	Alpha = 0x8,

	RGB = Red | Green | Blue,
	RGBA = Red | Green | Blue | Alpha
};

enum class BlendOperation : u8
{
	None,
	Add,
	Subtract,
};

enum class BlendFactor : u8
{
	Zero,
	One,
	SrcColor,
	OneMinusSrcColor,
	DstColor,
	OneMinusDstColor,
	SrcAlpha,
	OneMinusSrcAlpha,
	DstAlpha,
	OneMinusDstAlpha,
	ConstColor,
	OneMinusConstColor,
	ConstAlpha,
	OneMinusConstAlpha
};

inline ColorMask operator|(ColorMask e0, ColorMask e1)
{
	return (ColorMask)((u32)e0 | (u32)e1);
}

enum class SamplerAddressMode : u8
{
	Repeat,
	Clamp,
	Mirror
};

enum class SamplerFilter : u8
{
	Nearest,
	Linear
};

enum class SamplerMipmapMode : u8
{
	Nearest,
	Linear
};


