// Copyright 2020, Nathan Blane

#include "Math/Quat.hpp"
#include "Math/MathFunctions.hpp"
#include "GameObject.h"
#include "Model/Model.h"
#include "Input/Input.hpp"

#include "Shader/Material.h"
#include "Model/ModelFactory.h"

#include "Archiver/SkeletonHeader.h"

#include "Scene/GameWorld.hpp"
#include "RenderObjectManager.hpp"


GameObject::GameObject(GameWorld& gameWorld)
	: worldTransform(IDENTITY),
	position(),
	rotX(0.f),
	rotY(0.f),
	rotZ(0.f),
	scale(1, 1, 1),
	world(&gameWorld)
{

}

GameObject::GameObject(GameObject&& go) noexcept
	: worldTransform(std::move(go.worldTransform)),
	position(go.position),
	rotX(go.rotX),
	rotY(go.rotY),
	rotZ(go.rotZ),
	world(go.world),
	model(std::move(go.model))
{
}

// GameObject& GameObject::operator=(const GameObject& go)
// {
// 	if (this != &go)
// 	{
// 		world = go.world;
// 		model = MakeUnique<Model>(*go.model);
// 
// 		position = go.position;
// 		rotX = go.rotX;
// 		rotY = go.rotY;
// 		rotZ = go.rotZ;
// 		scale = go.scale;
// 
// 	}
// 
// 	return *this;
// }

GameObject& GameObject::operator=(GameObject&& go) noexcept
{
	if (this != &go)
	{
		worldTransform = std::move(go.worldTransform);
		model = std::move(go.model);

		position = go.position;
		rotX = go.rotX;
		rotY = go.rotY;
		rotZ = go.rotZ;
		scale = go.scale;
	}

	return *this;
}

void GameObject::SetModel(Model* m)
{
	if (model.IsValid())
	{
		world->UnregisterRenderInfo(*this);
	}

	model.Reset(m);

	world->RegisterRenderInfo(*this, model->GetRenderInfo());
}

void GameObject::ShowDebugVolume(bool show)
{
	if (debugShown != show)
	{
		if (show && debugVolume == nullptr)
		{
// 			VulkanShader* vertShader = GetShaderManager().CreateShader("default.vs.spv", ShaderStage::Vertex);
// 			vertShader->SetDescriptorInformation(*descriptorSetLayout);
// 			VulkanShader* fragShader = GetShaderManager().CreateShader("default.fs.spv", ShaderStage::Vertex);
// 			fragShader->SetDescriptorInformation(*descriptorSetLayout);

// 			Material* mat = new Material(*vertShader, *fragShader, "defaultTex", Color32::White());
// 			mat->EnableWireframe();
			//debugVolume = ModelFactory::CreateModel(MeshManager::FindMesh(Mesh::SphereName), mat);
		}
		else if (show && debugVolume != nullptr)
		{
			debugVolume->SetActive(true);
		}
		else if (!show && debugVolume != nullptr)
		{
			debugVolume->SetActive(false);
		}

		debugShown = show;
	}
}

void GameObject::ToggleDebugVolume()
{
	if (debugShown)
	{
		ShowDebugVolume(false);
	}
	else
	{
		ShowDebugVolume(true);
	}
}

void GameObject::ToggleDebugColor()
{
	static bool green = true;
	if (green)
	{
		Assert(debugVolume);
		debugVolume->GetMaterial()->SetColor(Color32(0, 1, 0, 1));
		green = false;
	}
	else
	{
		Assert(debugVolume);
		debugVolume->GetMaterial()->SetColor(Color32(1, 0, 0, 1));
		green = true;
	}
}

void GameObject::Update(NOT_USED float tick)
{
	Matrix4 Trans(TRANS, position);
	Quat Rot(ROT_XYZ, rotX, rotY, rotZ);
	Matrix4 Scale(SCALE, scale);

	//world * GetParent()->world
	worldTransform = Scale * Rot * Trans;

	if (model.IsValid())
	{
		model->SetWorld(worldTransform);
	}
	if (debugVolume != nullptr)
	{
		TransformDebugVolume();
	}
}

void GameObject::SetPos(const Vector4& pos)
{
	position = (pos);
}

void GameObject::SetPos(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void GameObject::SetRotation(float angleX, float angleY, float angleZ)
{
	rotX = Math::DegreesToRadians(angleX);
	rotY = Math::DegreesToRadians(angleY);
	rotZ = Math::DegreesToRadians(angleZ);
}

void GameObject::SetScale(float scaleX, float scaleY, float scaleZ)
{
	scale.x = scaleX;
	scale.y = scaleY;
	scale.z = scaleZ;
}

void GameObject::InputSetup()
{
}

void GameObject::MoveForward(int32 mod)
{
	position += Vector4((float)mod, 0, 0);
}

void GameObject::MoveRight(int32 /*mod*/)
{
}

void GameObject::MoveUp(int32 /*mod*/)
{
}

SphereBounds GameObject::GetCollisionInfo() const
{
	SphereBounds boundingSphere = model->GetMesh()->GetCollitionInfo();
	float radius = boundingSphere.radius;
	//Matrix local = Matrix(SCALE, radius, radius, radius) * Matrix(TRANS, info.boundingSphere.cntr);
	boundingSphere.position = boundingSphere.position * /*local **/ worldTransform;
	boundingSphere.radius = radius * scale.x;
	return boundingSphere;
}

void GameObject::SetDebugColor(Color32 color)
{
	Assert(model.IsValid());
	Assert(debugVolume);
	debugVolume->GetMaterial()->SetColor(color);
}

void GameObject::SetWorld(const Matrix4& newWorld)
{
	worldTransform = newWorld;
}

Matrix4 GameObject::GetWorld() const
{
	return worldTransform;
}

void GameObject::TransformDebugVolume()
{
	SphereBounds boundingSphere = model->GetMesh()->GetCollitionInfo();
	float radius = boundingSphere.radius * 2;
	Matrix4 local = Matrix4(SCALE, radius, radius, radius) * Matrix4(TRANS, boundingSphere.position);
	debugVolume->SetWorld(local * worldTransform);
}
