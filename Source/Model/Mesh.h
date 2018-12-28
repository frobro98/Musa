#pragma once

#include "Platform.h"
#include "Graphics.h"
#include "Containers/Array.h"
#include "Collision/Primitives/Sphere.h"

class ShaderProgram;

struct CollisionInfo
{
	Sphere boundingSphere;
};

class Mesh
{
public:
	static constexpr const char* SphereName = "Primitive-Sphere";
	static constexpr const char* BoxName = "Primitive-Box";
	static constexpr const char* PlaneName = "Primitive-Plane";
	static constexpr const char* PyramidName = "Primitive-Pyramid";

public:
	Mesh(Array<Vertex>&& vertList, Array<Face>&& faceList);
	Mesh(Array<Vertex>&& vertList, Array<Face>&& faceList, CollisionInfo* colInfo);
	Mesh(Array<Vertex>&& vertList, Array<Face>&& faceList, Array<VertexBoneWeights>&& skinningWeights, CollisionInfo* colInfo);

	~Mesh() = default;

	inline Array<Vertex> GetVertices() const { return vertices; }
	inline Array<VertexBoneWeights> GetSkinWeights() const { return skinWeights; }
	inline Array<Face> GetFaces() const { return faces; }
	inline CollisionInfo GetCollitionInfo() const { return *boundingVolume; }

private:
	Array<Vertex> vertices;
	Array<Face> faces;
	Array<VertexBoneWeights> skinWeights;

	CollisionInfo* boundingVolume;
};