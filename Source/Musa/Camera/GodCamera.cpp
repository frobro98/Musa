#include "GodCamera.hpp"
#include "Camera.h"
#include "Input/Input.hpp"
#include "Input/InputManager.h"

GodCamera::GodCamera(Camera& cam)
	: camera(cam)
{
	position = camera.GetPosition();
	cameraLookAt = camera.GetLookAt();
	cameraUp = camera.GetUp();

	SetupGodInputInput();
}

void GodCamera::Update(float /*tick*/)
{
	camera.SetOrientationAndPosition(cameraLookAt, position, cameraUp);

	// TODO - This should be done in the SetWorld method...
	world = Matrix(TRANS, position);
}

void GodCamera::SetupGodInputInput()
{
	GetInputManager().RegisterContinuousInput(KeyInput::Key_W, 1, this, &GodCamera::MoveCameraForward);
	GetInputManager().RegisterContinuousInput(KeyInput::Key_S, -1, this, &GodCamera::MoveCameraForward);

	GetInputManager().RegisterContinuousInput(KeyInput::Key_D, 1, this, &GodCamera::MoveCameraRight);
	GetInputManager().RegisterContinuousInput(KeyInput::Key_A, -1, this, &GodCamera::MoveCameraRight);

	GetInputManager().RegisterContinuousInput(KeyInput::Key_E, 1, this, &GodCamera::MoveCameraUp);
	GetInputManager().RegisterContinuousInput(KeyInput::Key_Q, -1, this, &GodCamera::MoveCameraUp);

	GetInputManager().RegisterMouseAxes(this, &GodCamera::CameraLookAtAdjust);
}

void GodCamera::MoveCameraForward(int32 mod)
{
	position += camera.GetForward() * (float)-mod * 1.5f;
	cameraLookAt += camera.GetForward() * (float)-mod * 1.5f;
}

void GodCamera::MoveCameraRight(int32 mod)
{
	position += camera.GetRight() * (float)mod * 1.5f;
	cameraLookAt += camera.GetRight() * (float)mod * 1.5f;
	//printf("Camera Right: (%f, %f, %f)\n", camera.GetRight().x, camera.GetRight().y, camera.GetRight().z);
}

void GodCamera::MoveCameraUp(int32 mod)
{
	position += camera.GetUp() * (float)mod * 1.5f;
	cameraLookAt += camera.GetUp() * (float)mod * 1.5f;
}

void GodCamera::CameraLookAtAdjust(float changeX, float changeY)
{
	const float speed = .1f;
	Quat quatX(ROT_AXIS_ANGLE, camera.GetRight(), -changeY * speed);
	Quat quatY(ROT_Y, -changeX * speed);

	Vector4 newLookAtDir = camera.GetForward() * quatX * quatY;
	newLookAtDir.Normalize();
	cameraLookAt = position - newLookAtDir;
	Vector4 up = camera.GetUp() * quatX * quatY;
	cameraUp = up;
}
