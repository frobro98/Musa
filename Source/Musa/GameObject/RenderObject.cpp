// Copyright 2020, Nathan Blane

#include "RenderObject.hpp"
#include "GameObject/GameObject.h"
#include "Mesh/MeshRenderInfo.hpp"
#include "Shader/MaterialRenderDescription.hpp"
#include "Scene/ScreenView.hpp"
#include "Graphics/UniformBuffers.h"
#include "Graphics/GraphicsInterface.hpp"

#include "Model/Model.h"
#include "Shader/Material.hpp"
#include "GameObject/GameObject.h"

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

void RenderObject::PullDataFromGameObject(const ScreenView& view)
{
	Assert(sceneObject != nullptr);
	Assert(gpuRenderInfo != nullptr);

	PullInfoForMesh();

	// TODO - This honestly doesn't belong here. RenderObjects won't exist later on anyways, so it doesn't really matter. Want to put a note here still though
	Material* mat = sceneObject->GetModel().GetMaterial();
	UniformBufferDescriptor primDescriptor = mat->GetUniformBufferConstant(PrimitiveParameterName);
	UniformBufferDescriptor viewDescriptor = mat->GetUniformBufferConstant(ViewParameterName);
	UniformBufferDescriptor matPropDescriptor = mat->GetUniformBufferConstant(MaterialPropertiesParameterName);
	mat->SetUniformBufferResource(primDescriptor, *gpuRenderInfo->transformBuffer);
	mat->SetUniformBufferResource(viewDescriptor, *view.view.viewBuffer);
}

void RenderObject::PullInfoForMesh()
{
	PrimUniformBuffer buffer;
	buffer.model = sceneObject->GetWorld();

	GetGraphicsInterface().PushBufferData(*gpuRenderInfo->transformBuffer, &buffer);
}
