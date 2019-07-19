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
	static Mesh* LoadPrimitive(Primitive primitiveMesh);
	static Mesh* LoadFrustumForCamera(class Camera* camera);

	static Mesh* LoadMeshFromPak(uint8* modelData, const char* modelName);
	static Mesh* LoadMeshFromPak(uint8* modelData, uint8* skinningData, const char* modelName);
	static void UnloadMesh(const char* modelName);
	static Mesh* FindMesh(const char* modelName);

private:

	Mesh* LoadCameraFrustum(class Camera* cam);
	Mesh* LoadSpherePrimitive();
	Mesh* LoadBoxPrimitive();
	Mesh* LoadPlanePrimitive();
	Mesh* LoadPyramidPrimitive();

	Mesh* LoadMeshInternalPak(uint8* modelData, const char* modelName);
	Mesh* LoadMeshInternalPak(uint8* modelData, uint8* skinningData, const char* modelName);
	void UnloadMeshInternal(const char* modelName);

	static MeshManager& Instance();

	MeshManager() = default;
	~MeshManager();

	struct MeshNode
	{
		MeshNode* next = nullptr;
		const char* meshName;
		Mesh* mesh = nullptr;
	};

	MeshNode* head = nullptr;
};