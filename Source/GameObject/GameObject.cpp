
#include "GameObject.h"
#include "Math/MathEngine.h"
#include "Graphics.h"
#include "Model/Model.h"
#include "Input/Input.hpp"

#include "Model/MeshManager.h"
#include "Model/Mesh.h"
#include "Shader/Material.h"
#include "Model/ModelFactory.h"

#include "Archiver/SkeletonHeader.h"

GameObject::GameObject()
	: world(IDENTITY),
	position(),
	rotX(0.f),
	rotY(0.f),
	rotZ(0.f),
	scale(1, 1, 1)
{

}

GameObject::GameObject(const GameObject& go)
	: world(go.world), 
	position(go.position),
	rotX(go.rotX),
	rotY(go.rotY),
	rotZ(go.rotZ),
	scale(go.scale),
	model(std::make_unique<Model>(*go.model))
{
}

GameObject::GameObject(GameObject&& go) noexcept
	: world(std::move(go.world)),
	position(go.position),
	rotX(go.rotX),
	rotY(go.rotY),
	rotZ(go.rotZ),
	model(std::move(go.model))
{
}

GameObject& GameObject::operator=(const GameObject& go)
{
	if (this != &go)
	{
		world = go.world;
		model = std::make_unique<Model>(*go.model);

		position = go.position;
		rotX = go.rotX;
		rotY = go.rotY;
		rotZ = go.rotZ;
		scale = go.scale;

	}

	return *this;
}

GameObject& GameObject::operator=(GameObject&& go) noexcept
{
	if (this != &go)
	{
		world = std::move(go.world);
		model = std::move(go.model);

		position = go.position;
		rotX = go.rotX;
		rotY = go.rotY;
		rotZ = go.rotZ;
		scale = go.scale;
	}

	return *this;
}

void GameObject::AssociateScene(Scene& scene_)
{
	scene = &scene_;
}

void GameObject::SetModel(Model* m)
{
	model.reset(m);
}

void GameObject::ShowDebugVolume(bool show)
{
	if (debugShown != show)
	{
		if (show && debugVolume == nullptr)
		{
			debugVolume = ModelFactory::CreateModel(MeshManager::FindMesh(Mesh::SphereName), new Material("wireframePipeline", nullptr, Color(1, 0, 0, 1)));
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
		assert(debugVolume);
		debugVolume->GetMaterial()->SetColor(Color(0, 1, 0, 1));
		green = false;
	}
	else
	{
		assert(debugVolume);
		debugVolume->GetMaterial()->SetColor(Color(1, 0, 0, 1));
		green = true;
	}
}

void GameObject::Update([[maybe_unused]] float tick)
{
	Matrix Trans(TRANS, position);
	Quat Rot(ROT_XYZ, rotX, rotY, rotZ);
	Matrix Scale(SCALE, scale);

	//world * GetParent()->world
	world = Scale * Rot * Trans;

	if (model != nullptr)
	{
		model->SetWorld(world);
	}
	if (debugVolume != nullptr)
	{
		TransformDebugVolume();
	}
}

void GameObject::SetPos(const Vector& pos)
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
	rotX = angleX;
	rotY = angleY;
	rotZ = angleZ;
}

void GameObject::SetScale(float scaleX, float scaleY, float scaleZ)
{
	scale.x = scaleX;
	scale.y = scaleY;
	scale.z = scaleZ;
}

void GameObject::InputSetup()
{
	// Move Forward
	GetInputManager().RegisterContinuousInput(KeyInput::Key_I, 1, this, &GameObject::MoveForward);
	// Move Backward
	GetInputManager().RegisterContinuousInput(KeyInput::Key_K, -1, this, &GameObject::MoveForward);
}

void GameObject::MoveForward(int32 mod)
{
	position += Vector((float)mod, 0, 0);
}

void GameObject::MoveRight(int32 /*mod*/)
{
}

void GameObject::MoveUp(int32 /*mod*/)
{
}

CollisionInfo GameObject::GetCollisionInfo() const
{
	CollisionInfo info = model->GetMesh()->GetCollitionInfo();
	float radius = info.boundingSphere.rad;
	//Matrix local = Matrix(SCALE, radius, radius, radius) * Matrix(TRANS, info.boundingSphere.cntr);
	info.boundingSphere.cntr = info.boundingSphere.cntr * /*local **/ world;
	info.boundingSphere.rad = radius * scale.x;
	return info;
}

void GameObject::SetDebugColor(Color color)
{
	assert(model);
	assert(debugVolume);
	debugVolume->GetMaterial()->SetColor(color);
}

void GameObject::SetWorld(const Matrix& newWorld)
{
	world = newWorld;
}

Matrix GameObject::GetWorld() const
{
	return world;
}

void GameObject::TransformDebugVolume()
{
	CollisionInfo info = model->GetMesh()->GetCollitionInfo();
	float radius = info.boundingSphere.rad * 2;
	Matrix local = Matrix(SCALE, radius, radius, radius) * Matrix(TRANS, info.boundingSphere.cntr);
	debugVolume->SetWorld(local * world);
}
