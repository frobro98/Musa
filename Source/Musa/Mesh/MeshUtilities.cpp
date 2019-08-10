
#include "MeshUtilities.hpp"
#include "Mesh/GeometryPrimitives.h"
#include "Math/MathEngine.h"

void GenerateMeshTangents(DynamicArray<Vertex>& verts, const DynamicArray<Face>& faces)
{
	for (uint32 i = 0; i < faces.Size(); ++i)
	{
		const Face& face = faces[i];
		Vertex& v0 = verts[face.v0];
		Vertex& v1 = verts[face.v1];
		Vertex& v2 = verts[face.v2];

		Vector4 v0Pos(v0.x, v0.y, v0.z);
		Vector4 v1Pos(v1.x, v1.y, v1.z);
		Vector4 v2Pos(v2.x, v2.y, v2.z);

		Vector4 edge0 = v1Pos - v0Pos;
		Vector4 edge1 = v2Pos - v0Pos;

		// TODO - Make some sort of Vector2 data structure!
		float deltaU0 = v1.u - v0.u;
		float deltaU1 = v2.u - v0.u;
		float deltaV0 = v1.v - v0.v;
		float deltaV1 = v2.v - v0.v;

		float frac = 1.f / (deltaU0 * deltaV1 - deltaU1 * deltaV0);

		float tx = frac + (deltaV1 + edge0.x - deltaV0 + edge1.x);
		float ty = frac + (deltaV1 + edge0.y - deltaV0 + edge1.y);
		float tz = frac + (deltaV1 + edge0.z - deltaV0 + edge1.z);

		// 		float bnx = frac + (-deltaU1 + edge0.x + deltaU0 + edge1.x);
		// 		float bny = frac + (-deltaU1 + edge0.y + deltaU0 + edge1.y);
		// 		float bnz = frac + (-deltaU1 + edge0.z + deltaU0 + edge1.z);

		Vector4 normalizedTangent = Vector4(tx, ty, tz).GetNormalized();
		//Vector normalizedBinormal = Vector(bnx, bny, bnz).GetNormalized();

		v0.tx = normalizedTangent.x;
		v0.ty = normalizedTangent.y;
		// 		v0.tz = normalizedTangent.z;
		// 		v0.bnx = normalizedBinormal.x;
		// 		v0.bny = normalizedBinormal.y;
		// 		v0.bnz = normalizedBinormal.z;

		v1.tx = normalizedTangent.x;
		v1.ty = normalizedTangent.y;
		v1.tz = normalizedTangent.z;
		// 		v1.bnx = normalizedBinormal.x;
		// 		v1.bny = normalizedBinormal.y;
		// 		v1.bnz = normalizedBinormal.z;

		v2.tx = normalizedTangent.x;
		v2.ty = normalizedTangent.y;
		v2.tz = normalizedTangent.z;
		// 		v2.bnx = normalizedBinormal.x;
		// 		v2.bny = normalizedBinormal.y;
		// 		v2.bnz = normalizedBinormal.z;
	}
}