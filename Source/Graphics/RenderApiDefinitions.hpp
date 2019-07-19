#pragma once

#include "EngineCore/Types/Intrinsics.hpp"

// TODO - move this enum to a better place
enum class LoadOperation : uint8
{
	Load,
	Clear,
	DontCare
};

// TODO - move this enum to a better place
enum class StoreOperation : uint8
{
	Store,
	DontCare
};

enum class CompareOperation : uint8
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

enum class StencilOperation : uint8
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

enum class PrimitiveTopology : uint32
{
	TriangleList,
	TriangleStrip,
	TriangleFan,
	LineList,
	LineStrip,
	PointList
};

enum class CullingMode : uint32
{
	None,
	Back,
	Front,
	FrontAndBack
};

enum class ShadingModel : uint32
{
	Lit,
	Unlit
};

enum class FillMode : uint32
{
	Full,
	Wireframe,
	Point
};

enum class ColorMask : uint8
{
	Red = 0x1,
	Green = 0x2,
	Blue = 0x4,
	Alpha = 0x8,

	RGB = Red | Green | Blue,
	RGBA = Red | Green | Blue | Alpha
};

enum class BlendOperation : uint8
{
	None,
	Add,
	Subtract,
};

enum class BlendFactor : uint8
{
	Zero,
	One,
};

inline ColorMask operator|(ColorMask e0, ColorMask e1)
{
	return (ColorMask)((uint32)e0 | (uint32)e1);
}