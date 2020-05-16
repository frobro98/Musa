// Copyright 2020, Nathan Blane

#include "RenderObject.hpp"
#include "GameObject/GameObject.h"
#include "Mesh/MeshRenderInfo.hpp"
#include "Shader/MaterialRenderInfo.hpp"
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
	PullInfoForMaterial();
}

void RenderObject::PullInfoForMesh()
{
	TransformationUniformBuffer buffer;
	buffer.model = sceneObject->GetWorld();

	GetGraphicsInterface().PushBufferData(*gpuRenderInfo->transformBuffer, &buffer);
}

void RenderObject::PullInfoForMaterial()
{
	MaterialRenderInfo* matInfo = gpuRenderInfo->meshMaterial;
	Assert(matInfo != nullptr);

	MaterialProperties props;
	props.diffuse = matInfo->baseColor;
	GetGraphicsInterface().PushBufferData(*matInfo->materialProperties, &props);
}
