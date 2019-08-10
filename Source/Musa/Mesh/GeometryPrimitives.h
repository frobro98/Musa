#pragma once

// TODO - Move these out of this mesh file
#include "Platform.h"

struct Vertex
{
	// NOTE - MSVC Internal compiler error occurs when these variables are all defaulted with '= 0'
	// Don't know how to approach this issue. This is the error given:
	//
	// INTERNAL COMPILER ERROR in 'C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\bin\HostX86\x86\CL.exe'
	// Please choose the Technical Support command on the Visual C++
	// Help menu, or open the Technical Support help file for more information
	//
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	float tx;
	float ty;
	float tz;
	float u;
	float v;

	friend bool operator==(const Vertex& vert0, const Vertex& vert1)
	{
		return (
			(vert0.x == vert1.x) &&
			(vert0.y == vert1.y) &&
			(vert0.z == vert1.z) &&
			(vert0.u == vert1.u) &&
			(vert0.v == vert1.v) &&
			(vert0.nx == vert1.nx) &&
			(vert0.ny == vert1.ny) &&
 			(vert0.nz == vert1.nz) &&
			(vert0.tx == vert1.tx) &&
			(vert0.ty == vert1.ty) &&
			(vert0.tz == vert1.tz)
		);
	}

	friend bool operator<(const Vertex& vert0, const Vertex& vert1)
	{
		return (
			(vert0.x < vert1.x) &&
			(vert0.y < vert1.y) &&
			(vert0.z < vert1.z) &&
			(vert0.nx < vert1.nx) &&
			(vert0.ny < vert1.ny) &&
			(vert0.nz < vert1.nz) &&
			(vert0.tx < vert1.tx) &&
			(vert0.ty < vert1.ty) &&
			(vert0.tz < vert1.tz) &&
			(vert0.u < vert1.u) &&
			(vert0.v < vert1.v)
		);
	}

	friend bool operator>(const Vertex& vert0, const Vertex& vert1)
	{
		return (
			(vert0.x > vert1.x) &&
			(vert0.y > vert1.y) &&
			(vert0.z > vert1.z) &&
			(vert0.nx > vert1.nx) &&
			(vert0.ny > vert1.ny) &&
			(vert0.nz > vert1.nz) &&
			(vert0.tx > vert1.tx) &&
			(vert0.ty > vert1.ty) &&
			(vert0.tz > vert1.tz) &&
			(vert0.u > vert1.u) &&
			(vert0.v > vert1.v)
			);
	}
};

struct PrimitiveVertex
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	float tx;
	float ty;
	float tz;
	
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