
#include "Mesh.h"
#include "DirectoryLocations.h"
#include "File/FileCore/File.h"
#include "Platform.h"
#include "ModelFileHeader.h"

#define offset(s, m) ((void *)((unsigned int)(&s[0].m) - (unsigned int)(s)))

Mesh::Mesh(Array<Vertex>&& vertList, Array<Face>&& faceList)
	: vertices(vertList),
	faces(faceList)
{
}

Mesh::Mesh(Array<Vertex>&& vertList, Array<Face>&& faceList, CollisionInfo* colInfo)
	: vertices(vertList),
	faces(faceList),
	boundingVolume(colInfo)
{
}

Mesh::Mesh(Array<Vertex>&& vertList, Array<Face>&& faceList, Array<VertexBoneWeights>&& weights, CollisionInfo * colInfo)
	: vertices(vertList),
	faces(faceList),
	skinWeights(weights),
	boundingVolume(colInfo)
{
}
