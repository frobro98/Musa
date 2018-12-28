#pragma once

class Model;
class Renderer;
class RenderingEntity;

class RenderingEntityManager
{
public:
	static RenderingEntity* CreateOrGetEntity(Renderer* renderer, Model* model);
	static void RemoveEntity(Model* model);

private:
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
	Renderer* renderer;
};