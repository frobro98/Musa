
#include "ModelFactory.h"
#include "Model.h"
#include "Renderer/Renderer.h"
#include "Renderer/DrawList.h"

void ModelFactory::Initialize(Renderer* renderer_)
{
	Instance().renderer = renderer_;
}

Model* ModelFactory::CreateModel(Mesh * mesh, Material * material)
{
	Model* model = new Model(mesh, material);
	DrawList* mainDrawList = Instance().renderer->GetDrawList();
	mainDrawList->AddToRenderList(model);
	model->SetOwningDrawList(mainDrawList);
	
	return model;
}

void ModelFactory::DestroyModel(Model * model)
{
	delete model;
}

ModelFactory& ModelFactory::Instance()
{
	static ModelFactory mfInstance;
	return mfInstance;
}
