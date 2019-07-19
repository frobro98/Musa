#include "PlatformDefinitions.h"
#include "CameraManager.h"
#include "Camera.h"
#include "EngineCore/String/CStringUtilities.hpp"

void CameraManager::AddCamera(Camera * camera, const char * cameraName)
{
	CameraNode node;
	node.cameraName = cameraName;
	node.camera = camera;
	cameras.Add(node);
}

void CameraManager::RemoveCamera(const char* cameraName)
{
	for (uint32 i = 0; i < cameras.Size(); ++i)
	{
		if (Strcmp(cameras[i].cameraName, cameraName) == 0)
		{
			cameras.Remove(i);
			break;
		}
	}
}

Camera* CameraManager::FindCamera(const char * cameraName)
{
	for (const auto& camera : cameras)
	{
		if (Strcmp(cameraName, camera.cameraName) == 0)
		{
			return camera.camera;
		}
	}

	return nullptr;
}

void CameraManager::SetActiveCamera(const char * cameraName)
{
	Camera* camera = FindCamera(cameraName);
	Assert(camera != nullptr);
	activeCamera = camera;
}

Camera* CameraManager::GetActiveCamera()
{
	return activeCamera;
}

void CameraManager::Resize(int width, int height, float aspectRatio)
{
	for (auto& camera : cameras)
	{
		Camera& cam = *camera.camera;
		IntRect viewport = { 0, 0, width, height };
		cam.SetViewport(viewport);
		cam.SetAspectRatio(aspectRatio);
	}
}

CameraManager::~CameraManager()
{
}

CameraManager& GetCameraManager()
{
	static CameraManager cmInstance;
	return cmInstance;
}
