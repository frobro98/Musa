#pragma once

class GameObject;
struct MeshRenderInfo;
struct View;

struct RenderObject
{
	void SetGameObject(const GameObject& obj, MeshRenderInfo& meshInfo);
	void ResetGameObject(); 

	void PullDataFromGameObject(const View& currentView);

	const GameObject* sceneObject = nullptr;
	MeshRenderInfo* gpuRenderInfo = nullptr;

private:
	void PullInfoForMesh(const View& currentView);
	void PullInfoForMaterial();
};