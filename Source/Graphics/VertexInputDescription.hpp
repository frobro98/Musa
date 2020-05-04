#pragma once

#include <type_traits>
// TODO - Remove the include of std::array<>...
#include <array>

#include "EngineCore/Types/Intrinsics.hpp"
#include "Musa/Mesh/GeometryPrimitives.h"
#include "Utilities/TemplateUtils.hpp"

enum class VertexInputType : uint16
{
	F32_1,
	F32_2,
	F32_3,
	F32_4
};

struct VertexInputDescription
{
	VertexInputType type;
	uint16 stride;
	uint16 offset;
	uint8 binding;
	uint8 location;

	friend bool operator==(const VertexInputDescription& lhs, const VertexInputDescription& rhs)
	{
		return lhs.type == rhs.type &&
			lhs.stride == rhs.stride &&
			lhs.offset == rhs.offset &&
			lhs.binding == rhs.binding &&
			lhs.location == rhs.location;
	}
};

constexpr uint32 MaxVertexInputDescriptions = 6;

struct VertexInputDescriptionList
{
	using InputDescriptionList = std::array<VertexInputDescription, MaxVertexInputDescriptions>;
	InputDescriptionList inputDescs;
	uint32 vertexInputCount;

	friend bool operator==(const VertexInputDescriptionList& lhs, const VertexInputDescriptionList& rhs)
	{
		if (lhs.vertexInputCount == rhs.vertexInputCount)
		{
			uint32 vertexInputCount = lhs.vertexInputCount;
			bool result = true;
			for (uint32 i = 0; i < vertexInputCount; ++i)
			{
				result &= lhs.inputDescs[i] == rhs.inputDescs[i];
			}

			return result;
		}
		return false;
	}
};

template<class MeshType>
inline const VertexInputDescriptionList GetVertexInput()
{ 
	static_assert(always_false_v<MeshType>, "The type passed in doesn't have a vertex input defined");
}

template<>
inline const VertexInputDescriptionList GetVertexInput<Vertex>()
{
	return VertexInputDescriptionList
	{
		// List
		{{
			{
				VertexInputType::F32_3, // Type
				sizeof(Vertex), // Stride
				offsetof(Vertex, position), // Offset
				0, // Binding
				0 // Location
			},

			{
				VertexInputType::F32_3,
				sizeof(Vertex),
				offsetof(Vertex, normal),
				0,
				1
			},

			{
				VertexInputType::F32_3,
				sizeof(Vertex),
				offsetof(Vertex, tangent),
				0,
				2
			},

			{
				VertexInputType::F32_2,
				sizeof(Vertex),
				offsetof(Vertex, texCoords),
				0,
				3
			}
		}},
		// Count
		4
	};
}

template<>
inline const VertexInputDescriptionList GetVertexInput<PrimitiveVertex>()
{
	return VertexInputDescriptionList
	{
		// List
		{{
			{
				VertexInputType::F32_3, // Type
				sizeof(PrimitiveVertex), // Stride
				offsetof(PrimitiveVertex, position), // Offset
				0, // Binding
				0 // Location
			},

			{
				VertexInputType::F32_2,
				sizeof(PrimitiveVertex),
				offsetof(PrimitiveVertex, texCoords),
				0,
				1
			},

			{
				VertexInputType::F32_4,
				sizeof(PrimitiveVertex),
				offsetof(PrimitiveVertex, color),
				0,
				2
			}
		}},
		// Count
		3
	};
}
