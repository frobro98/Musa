// Copyright 2020, Nathan Blane

#pragma once

class GameObject;
struct MeshRenderInfo;
class ScreenView;

struct RenderObject
{
	void SetGameObject(const GameObject& obj, MeshRenderInfo& meshInfo);
	void ResetGameObject(); 

	void PullDataFromGameObject(const ScreenView& view);

	const GameObject* sceneObject = nullptr;
	MeshRenderInfo* gpuRenderInfo = nullptr;

private:
	void PullInfoForMesh();
};