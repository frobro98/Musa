#pragma once

class Model;
class Renderer;
class RenderingEntity;
struct MeshRenderInfo;

class RenderingEntityManager
{
public:
	static RenderingEntity* CreateOrGetEntity(Renderer* renderer, MeshRenderInfo& meshInfo);
	static void RemoveEntity(Model* model);

private:
	static RenderingEntity* CreateOrGetUnlit(Renderer* renderer, MeshRenderInfo& meshInfo);
	static RenderingEntity* CreateOrGetLit(Renderer* renderer, MeshRenderInfo& meshInfo);

	static RenderingEntityManager& Instance();

	RenderingEntityManager() = default;
	~RenderingEntityManager();

private:
	struct EntityNode
	{
		EntityNode* next = nullptr;
		RenderingEntity* renderingEntity;
	};

	EntityNode* head = nullptr;
	EntityNode* unlitHead = nullptr;
	Renderer* renderer;
};