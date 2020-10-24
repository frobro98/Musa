// Copyright 2020, Nathan Blane

#include "Mesh.h"
#include "File/DirectoryLocations.hpp"
#include "File/FileSystem.hpp"
#include "Model/ModelFileHeader.h"
#include "Graphics/GraphicsInterface.hpp"

#define offset(s, m) ((void *)((unsigned int)(&s[0].m) - (unsigned int)(s)))

Mesh::Mesh(const DynamicArray<Vertex>& vertList, const DynamicArray<Face>& faceList)
	: vertexBuffer(GetGraphicsInterface().CreateVertexBuffer(vertList)),
	indexBuffer(GetGraphicsInterface().CreateIndexBuffer(faceList))
{

}

Mesh::Mesh(const DynamicArray<Vertex>& vertList, const DynamicArray<Face>& faceList, const SphereBounds& colInfo)
	: vertexBuffer(GetGraphicsInterface().CreateVertexBuffer(vertList)),
	indexBuffer(GetGraphicsInterface().CreateIndexBuffer(faceList)),
	boundingSphere(colInfo)
{
}

Mesh::Mesh(const DynamicArray<Vertex>& vertList, const DynamicArray<Face>& faceList, const DynamicArray<VertexBoneWeights>& /*weights*/, const SphereBounds& colInfo)
	: vertexBuffer(GetGraphicsInterface().CreateVertexBuffer(vertList)),
	indexBuffer(GetGraphicsInterface().CreateIndexBuffer(faceList)),
	boundingSphere(colInfo)
{
}
