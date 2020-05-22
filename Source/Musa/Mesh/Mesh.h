// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "Containers/DynamicArray.hpp"
#include "Math/BoundsVolumes.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Graphics/GeometryPrimitives.hpp"

struct NativeVertexBuffer;
struct NativeIndexBuffer;

class Mesh
{
public:
	static constexpr const char* SphereName = "Primitive-Sphere";
	static constexpr const char* BoxName = "Primitive-Box";
	static constexpr const char* PlaneName = "Primitive-Plane";
	static constexpr const char* PyramidName = "Primitive-Pyramid";

public:
	Mesh(DynamicArray<Vertex>&& vertList, DynamicArray<Face>&& faceList);
	Mesh(DynamicArray<Vertex>&& vertList, DynamicArray<Face>&& faceList, const SphereBounds& colInfo);
	Mesh(DynamicArray<Vertex>&& vertList, DynamicArray<Face>&& faceList, DynamicArray<VertexBoneWeights>&& skinningWeights, const SphereBounds& colInfo);

	inline DynamicArray<Vertex> GetVertices() const { return vertices; }
	inline DynamicArray<VertexBoneWeights> GetSkinWeights() const { return skinWeights; }
	inline DynamicArray<Face> GetFaces() const { return faces; }
	inline SphereBounds GetCollitionInfo() const { return boundingSphere; }

	inline NativeVertexBuffer& GetVertexBuffer() { return *vertexBuffer; }
	inline NativeIndexBuffer& GetIndexBuffer() { return *indexBuffer; }

private:
	DynamicArray<Vertex> vertices;
	DynamicArray<Face> faces;
	DynamicArray<VertexBoneWeights> skinWeights;

	UniquePtr<NativeVertexBuffer> vertexBuffer;
	UniquePtr<NativeIndexBuffer> indexBuffer;

	SphereBounds boundingSphere;
};