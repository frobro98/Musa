
#include "Mesh.h"
#include "DirectoryLocations.h"
#include "File/FileCore/File.h"
#include "Platform.h"
#include "Model/ModelFileHeader.h"

// TODO - Remove this Vulkan reference!!!!!
#include "Graphics/Vulkan/VulkanMemory.h"

#define offset(s, m) ((void *)((unsigned int)(&s[0].m) - (unsigned int)(s)))

Mesh::Mesh(DynamicArray<Vertex>&& vertList, DynamicArray<Face>&& faceList)
	: vertices(std::move(vertList)),
	faces(std::move(faceList)),
	vertexBuffer(VulkanMemory::CreateVertexBuffer(vertices)),
	indexBuffer(VulkanMemory::CreateIndexBuffer(faces))
{

}

Mesh::Mesh(DynamicArray<Vertex>&& vertList, DynamicArray<Face>&& faceList, const SphereBounds& colInfo)
	: vertices(vertList),
	faces(faceList),
	vertexBuffer(VulkanMemory::CreateVertexBuffer(vertices)),
	indexBuffer(VulkanMemory::CreateIndexBuffer(faces)),
	boundingSphere(colInfo)
{
}

Mesh::Mesh(DynamicArray<Vertex>&& vertList, DynamicArray<Face>&& faceList, DynamicArray<VertexBoneWeights>&& weights, const SphereBounds& colInfo)
	: vertices(vertList),
	faces(faceList),
	skinWeights(weights),
	vertexBuffer(VulkanMemory::CreateVertexBuffer(vertices)),
	indexBuffer(VulkanMemory::CreateIndexBuffer(faces)),
	boundingSphere(colInfo)
{
}
