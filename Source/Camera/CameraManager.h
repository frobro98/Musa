#pragma once

class Camera;

class CameraManager
{
public:
	static void AddCamera(Camera* camera, const char* cameraName);
	static void RemoveCamera(const char* camera);
	static Camera* FindCamera(const char* cameraName);
	
	static void SetActiveCamera(const char* cameraName);
	static Camera* GetActiveCamera();

	static void Update();

	static void Resize(int width, int height, float aspectRatio);

private:

	void AddCameraInternal(Camera* camera, const char* cameraName);
	void RemoveCameraInternal(const char* cameraName);

	void ResizeCameras(int width, int height, float aspectRatio);

	static CameraManager& Instance();

	CameraManager() = default;
	~CameraManager();

	struct CameraNode
	{
		CameraNode* next = nullptr;
		const char* cameraName;
		Camera* camera = nullptr;
	};

	CameraNode* head = nullptr;
	Camera* activeCamera = nullptr;
};

