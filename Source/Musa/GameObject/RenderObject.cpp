// Copyright 2020, Nathan Blane

#include "RenderObject.hpp"
#include "GameObject/GameObject.h"
#include "Mesh/MeshRenderInfo.hpp"
#include "Shader/MaterialRenderDescription.hpp"
#include "Scene/ScreenView.hpp"
#include "Graphics/UniformBuffers.h"
#include "Graphics/GraphicsInterface.hpp"

void RenderObject::SetGameObject(const GameObject& obj, MeshRenderInfo& meshInfo)
{
	sceneObject = &obj;
	gpuRenderInfo = &meshInfo;
}

void RenderObject::ResetGameObject()
{
	sceneObject = nullptr;
	gpuRenderInfo = nullptr;
}

void RenderObject::PullDataFromGameObject()
{
	Assert(sceneObject != nullptr);
	Assert(gpuRenderInfo != nullptr);

	PullInfoForMesh();
}

void RenderObject::PullInfoForMesh()
{
	TransformationUniformBuffer buffer;
	buffer.model = sceneObject->GetWorld();

	GetGraphicsInterface().PushBufferData(*gpuRenderInfo->transformBuffer, &buffer);
}
