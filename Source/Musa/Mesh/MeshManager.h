#pragma once

#include "Platform.h"

class Mesh;

enum class Primitive
{
	Sphere,
	Box,
	Plane,
	Pyramid
};

class MeshManager
{
public:

	void Initialize();
	void Deinitialize();

	Mesh* LoadPrimitive(Primitive primitiveMesh);
	Mesh* LoadFrustumForCamera(class Camera* camera);

	Mesh* LoadMeshFromPak(uint8* modelData, const char* modelName);
	Mesh* LoadMeshFromPak(uint8* modelData, uint8* skinningData, const char* modelName);
	void UnloadMesh(const char* modelName);
	Mesh* FindMesh(const char* modelName);

private:

	Mesh* LoadCameraFrustum(class Camera* cam);
	Mesh* LoadSpherePrimitive();
	Mesh* LoadBoxPrimitive();
	Mesh* LoadPlanePrimitive();
	Mesh* LoadPyramidPrimitive();

	Mesh* LoadMeshInternalPak(uint8* modelData, const char* modelName);
	Mesh* LoadMeshInternalPak(uint8* modelData, uint8* skinningData, const char* modelName);
	void UnloadMeshInternal(const char* modelName);

	struct MeshNode
	{
		MeshNode* next = nullptr;
		const char* meshName = nullptr;
		Mesh* mesh = nullptr;
	};

	MeshNode* head = nullptr;
};

MeshManager& GetMeshManager();
