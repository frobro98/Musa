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
// 	float bnx;
// 	float bny;
// 	float bnz;
	float u;
	float v;

	bool operator==(const Vertex& vert)
	{
		return (
			(x == vert.x) &&
			(y == vert.y) &&
			(z == vert.z) &&
			(u == vert.u) &&
			(v == vert.v) &&
			(nx == vert.nx) &&
			(ny == vert.ny) &&
 			(nz == vert.nz) &&
			(tx == vert.tx) &&
			(ty == vert.ty) &&
			(tz == vert.tz) //&&
// 			(bnx == vert.bnx) &&
// 			(bny == vert.bny) &&
// 			(bnz == vert.bnz)
		);
	}

	bool operator<(const Vertex& vert)
	{
		// X position
		if (x < vert.x)
		{
			return true;
		}
		else if (x > vert.x)
		{
			return false;
		}

		// Y Position
		if (y < vert.y)
		{
			return true;
		}
		else if (y > vert.y)
		{
			return false;
		}

		// Z Position
		if (z < vert.z)
		{
			return true;
		}
		else if (z > vert.z)
		{
			return false;
		}

		// Normal X
		if (nx < vert.nx)
		{
			return true;
		}
		else if (nx > vert.nx)
		{
			return false;
		}

		// Normal Y
		if (ny < vert.ny)
		{
			return true;
		}
		else if (ny > vert.ny)
		{
			return false;
		}

		// Normal Z
		if (nz < vert.nz)
		{
			return true;
		}
		else if (nz > vert.nz)
		{
			return false;
		}

		// U Coordinate
		if (u < vert.u)
		{
			return true;
		}
		else if (u > vert.u)
		{
			return false;
		}

		// V Coordinate
		if (v < vert.v)
		{
			return true;
		}
		else if (v > vert.v)
		{
			return false;
		}

		return false;
	}
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