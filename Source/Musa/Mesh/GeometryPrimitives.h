#pragma once

#include "Platform.h"
#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Color.hpp"

	// NOTE - MSVC Internal compiler error occurs when these variables are all defaulted with '= 0'
	// Don't know how to approach this issue. This is the error given:
	//
	// INTERNAL COMPILER ERROR in 'C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\bin\HostX86\x86\CL.exe'
	// Please choose the Technical Support command on the Visual C++
	// Help menu, or open the Technical Support help file for more information
	//
struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector3 tangent;
	Vector2 texCoords;

	friend bool operator==(const Vertex& vert0, const Vertex& vert1)
	{
		return vert0.position == vert1.position &&
			vert0.normal == vert1.normal &&
			vert0.tangent == vert1.tangent &&
			vert0.texCoords == vert1.texCoords;
	}

	friend bool operator<(const Vertex& vert0, const Vertex& vert1)
	{
		return vert0.position < vert1.position &&
			vert0.normal < vert1.normal &&
			vert0.tangent < vert1.tangent &&
			vert0.texCoords < vert1.texCoords;
	}

	friend bool operator>(const Vertex& vert0, const Vertex& vert1)
	{
		return vert0.position > vert1.position &&
			vert0.normal > vert1.normal &&
			vert0.tangent > vert1.tangent &&
			vert0.texCoords > vert1.texCoords;
	}
};

struct PrimitiveVertex
{
	Vector3 position;
	Vector3 normal;
	Vector3 tangent;
	
};

struct VertexBoneWeights
{
	static constexpr uint32 MaxWeightInfluences = 4;

	float weights[MaxWeightInfluences] = { 0.f, 0.f, 0.f, 0.f };
	uint32 boneIndices[MaxWeightInfluences] = { 0, 0, 0, 0 };
};

struct Face
{
	uint32 v0;
	uint32 v1;
	uint32 v2;
};