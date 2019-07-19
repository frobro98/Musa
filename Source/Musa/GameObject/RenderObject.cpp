
#include "RenderObject.hpp"
#include "GameObject/GameObject.h"
#include "Mesh/MeshSceneInfo.hpp"
#include "Shader/MaterialRenderInfo.hpp"
#include "Scene/ScreenView.hpp"
#include "Graphics/UniformBuffer.h"

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

void RenderObject::PullDataFromGameObject(const View& currentView)
{
	Assert(sceneObject != nullptr);
	Assert(gpuRenderInfo != nullptr);

	PullInfoForMesh(currentView);
	PullInfoForMaterial();
}

void RenderObject::PullInfoForMesh(const View& currentView)
{
	TransformationUniformBuffer buffer;
	buffer.model = sceneObject->GetWorld();
	buffer.view = currentView.transforms.viewMatrix;
	buffer.projection = currentView.transforms.projectionMatrix;

	gpuRenderInfo->transformBuffer->UpdateUniforms(&buffer);
}

void RenderObject::PullInfoForMaterial()
{
	MaterialRenderInfo* matInfo = gpuRenderInfo->meshMaterial;
	Assert(matInfo != nullptr);

	MaterialProperties props;
	props.diffuse = matInfo->baseColor;
	matInfo->materialProperties->UpdateUniforms(&props);
}
