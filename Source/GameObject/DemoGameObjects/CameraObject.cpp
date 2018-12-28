#include "CameraObject.h"
#include "Camera/Camera.h"
#include "Camera/CameraManager.h"
#include "Input/Input.hpp"
#include "Model/ModelFactory.h"
#include "Model/MeshManager.h"
#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Shader/Material.h"

CameraObject::CameraObject(const char* camName)
	:cameraName(camName)
{
	camera = CameraManager::FindCamera(cameraName);
	assert(camera != nullptr);
	position = camera->GetPosition();
	lookat = camera->GetLookAt();
	//camera->SetClearColor(Color{.7f, .7f, .8f, 1.f});
}

void CameraObject::SetCamera()
{
	CameraManager::SetActiveCamera(cameraName);
}

void CameraObject::Update(float /*tick*/)
{
	camera->SetOrientationAndPosition(lookat, position, camera->GetUp());

	world =  Matrix(TRANS, position);

	if (model != nullptr)
	{
		model->SetWorld(world);
	}
	if (debugVolume != nullptr)
	{
		TransformDebugVolume();
	}
}

void CameraObject::ShowDebugVolume(bool show)
{
	if (debugShown != show)
	{
		if (show && debugVolume == nullptr)
		{
			debugVolume = ModelFactory::CreateModel(MeshManager::LoadFrustumForCamera(camera), new Material("wireframePipeline", nullptr, Color(1, 1, 1, 1)));
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

void CameraObject::SetupGodCam()
{
	GetInputManager().RegisterContinuousInput(KeyInput::Key_W, 1, this, &CameraObject::MoveForward);
	GetInputManager().RegisterContinuousInput(KeyInput::Key_S, -1, this, &CameraObject::MoveForward);

	GetInputManager().RegisterContinuousInput(KeyInput::Key_D, 1, this, &CameraObject::MoveRight);
	GetInputManager().RegisterContinuousInput(KeyInput::Key_A, -1, this, &CameraObject::MoveRight);

	GetInputManager().RegisterMouseAxes(this, &CameraObject::LookAtAdjust);
}

void CameraObject::MoveForward(int32 mod)
{
	position += camera->GetForward() * (float)-mod * 1.5f;
	lookat += camera->GetForward() * (float)-mod * 1.5f;
}

void CameraObject::MoveRight(int32 mod)
{
	position += camera->GetRight() * (float)mod * 1.5f;
	lookat += camera->GetRight() * (float)mod * 1.5f;
}

void CameraObject::LookAtAdjust(float changeX, float changeY)
{
	const float speed = .1f;
 	Quat quatX(ROT_AXIS_ANGLE, camera->GetRight(), -changeY * speed);
 	Quat quatY(ROT_Y, -changeX * speed);

 	Vector newLookAtDir = camera->GetForward() * quatX * quatY;
 	newLookAtDir.Normalize();
 	lookat = position - newLookAtDir;
	Vector up = camera->GetUp() * quatX * quatY;
	camera->SetOrientationAndPosition(lookat, position, up);
 	
}

void CameraObject::TransformDebugVolume()
{
	debugVolume->SetWorld(world);
}
