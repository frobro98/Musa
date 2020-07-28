// Copyright 2020, Nathan Blane

#include "Mesh.h"
#include "File/DirectoryLocations.hpp"
#include "File/FileSystem.hpp"
#include "Model/ModelFileHeader.h"
#include "Graphics/GraphicsInterface.hpp"

#define offset(s, m) ((void *)((unsigned int)(&s[0].m) - (unsigned int)(s)))

Mesh::Mesh(DynamicArray<Vertex>&& vertList, DynamicArray<Face>&& faceList)
	: vertices(std::move(vertList)),
	faces(std::move(faceList)),
	vertexBuffer(GetGraphicsInterface().CreateVertexBuffer(vertices)),
	indexBuffer(GetGraphicsInterface().CreateIndexBuffer(faces))
{

}

Mesh::Mesh(DynamicArray<Vertex>&& vertList, DynamicArray<Face>&& faceList, const SphereBounds& colInfo)
	: vertices(std::move(vertList)),
	faces(std::move(faceList)),
	vertexBuffer(GetGraphicsInterface().CreateVertexBuffer(vertices)),
	indexBuffer(GetGraphicsInterface().CreateIndexBuffer(faces)),
	boundingSphere(colInfo)
{
}

Mesh::Mesh(DynamicArray<Vertex>&& vertList, DynamicArray<Face>&& faceList, DynamicArray<VertexBoneWeights>&& weights, const SphereBounds& colInfo)
	: vertices(std::move(vertList)),
	faces(std::move(faceList)),
	skinWeights(weights),
	vertexBuffer(GetGraphicsInterface().CreateVertexBuffer(vertices)),
	indexBuffer(GetGraphicsInterface().CreateIndexBuffer(faces)),
	boundingSphere(colInfo)
{
}
