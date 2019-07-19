#pragma once

#include "Platform.h"
#include "Graphics.h"
#include "Containers/DynamicArray.hpp"
#include "Math/BoundsVolumes.hpp"

#include "Graphics/Vulkan/VulkanVertexBuffer.h"
#include "Graphics/Vulkan/VulkanIndexBuffer.h"

class ShaderProgram;

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

	inline VertexBuffer& GetVertexBuffer() { return *vertexBuffer; }
	inline IndexBuffer& GetIndexBuffer() { return *indexBuffer; }

private:
	DynamicArray<Vertex> vertices;
	DynamicArray<Face> faces;
	DynamicArray<VertexBoneWeights> skinWeights;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	SphereBounds boundingSphere;
};