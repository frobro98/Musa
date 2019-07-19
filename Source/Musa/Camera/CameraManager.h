#pragma once

#include "Containers/DynamicArray.hpp"

class Camera;

class CameraManager
{
public:
	void AddCamera(Camera* camera, const char* cameraName);
	void RemoveCamera(const char* camera);
	Camera* FindCamera(const char* cameraName);
	
	void SetActiveCamera(const char* cameraName);
	Camera* GetActiveCamera();

	void Resize(int width, int height, float aspectRatio);

	friend CameraManager& GetCameraManager();

private:

	CameraManager() = default;
	~CameraManager();

	struct CameraNode
	{
		const char* cameraName;
		Camera* camera = nullptr;
	};

	DynamicArray<CameraNode> cameras;
	Camera* activeCamera = nullptr;
};

CameraManager& GetCameraManager();

