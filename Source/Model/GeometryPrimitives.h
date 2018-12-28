#pragma once

// TODO - Move these out of this mesh file
#include "Platform.h"

struct Vertex
{
	float x = 0;
	float y = 0;
	float z = 0;
	float nx = 0;
	float ny = 0;
	float nz = 0;
	float u = 0;
	float v = 0;

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
			(nz == vert.nz)
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