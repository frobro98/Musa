#include "PlatformDefinitions.h"
#include "CameraManager.h"
#include "Camera.h"


void CameraManager::AddCamera(Camera * camera, const char * cameraName)
{
	Instance().AddCameraInternal(camera, cameraName);
}

void CameraManager::RemoveCamera(const char * cameraName)
{
	Instance().RemoveCameraInternal(cameraName);
}

Camera * CameraManager::FindCamera(const char * cameraName)
{
	CameraNode* current = Instance().head;
	while (current != nullptr)
	{
		if (strcmp(cameraName, current->cameraName) == 0)
		{
			return current->camera;
		}
		current = current->next;
	}

	return nullptr;
}

void CameraManager::SetActiveCamera(const char * cameraName)
{
	Camera* camera = FindCamera(cameraName);
	assert(camera != nullptr);
	Instance().activeCamera = camera;
}

Camera * CameraManager::GetActiveCamera()
{
	return Instance().activeCamera;
}

void CameraManager::Update()
{
	CameraNode* current = Instance().head;
	while (current != nullptr)
	{
		current->camera->Update();
		current = current->next;
	}
}

void CameraManager::Resize(int width, int height, float aspectRatio)
{
	Instance().ResizeCameras(width, height, aspectRatio);
}

void CameraManager::AddCameraInternal(Camera* camera, const char* cameraName)
{
	CameraNode* node = new CameraNode;
	node->cameraName = cameraName;
	node->camera = camera;

	node->next = head;
	head = node;
}

void CameraManager::RemoveCameraInternal(const char* cameraName)
{
	CameraNode* current = head;
	while (current != nullptr)
	{
		if (strcmp(current->next->cameraName, cameraName) == 0)
		{
			CameraNode* node = current->next;
			current->next = current->next->next;
			delete node->camera;
			delete node;
			break;
		}

		current = current->next;
	}
}

void CameraManager::ResizeCameras(int width, int height, float aspectRatio)
{
	CameraNode* current = head;
	while (current != nullptr)
	{
		Camera* camera = current->camera;
		Viewport viewport = { 0, 0, width, height };
		camera->SetViewport(viewport);
		camera->SetAspectRatio(aspectRatio);

		current = current->next;
	}
}

CameraManager & CameraManager::Instance()
{
	static CameraManager cmInstance;
	return cmInstance;
}

CameraManager::~CameraManager()
{
}
