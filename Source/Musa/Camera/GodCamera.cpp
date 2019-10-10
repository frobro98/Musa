#include "GodCamera.hpp"
#include "Camera.h"
#include "Input/Input.hpp"

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

void GodCamera::InputCallback(const FrameInputs& inputs)
{
	for (const auto& action : inputs.actions)
	{
		UNUSED(action);
	}

	float32 changeX = 0, changeY = 0;
	for (const auto& range : inputs.ranges)
	{
		if (range.input->input == Inputs::Mouse_XAxis)
		{
			changeX = range.rangeValue;
		}
		if (range.input->input == Inputs::Mouse_YAxis)
		{
			changeY = range.rangeValue;
		}
	}

	CameraLookAtAdjust(changeX, changeY);

	for (const auto& state : inputs.states)
	{
		if (state->input == Inputs::Key_W)
		{
			MoveCameraForward(1);
		}
		else if (state->input == Inputs::Key_S)
		{
			MoveCameraForward(-1);
		}
		else if (state->input == Inputs::Key_A)
		{
			MoveCameraRight(-1);
		}
		else if (state->input == Inputs::Key_D)
		{
			MoveCameraRight(1);
		}
		else if (state->input == Inputs::Key_E)
		{
			MoveCameraUp(1);
		}
		else if (state->input == Inputs::Key_Q)
		{
			MoveCameraUp(-1);
		}
	}
}

void GodCamera::SetupGodInputInput()
{
}

void GodCamera::MoveCameraForward(int32 mod)
{
	const float32 speed = .5f;
	position += camera.GetForward() * (float)-mod * speed;
	cameraLookAt += camera.GetForward() * (float)-mod * speed;
}

void GodCamera::MoveCameraRight(int32 mod)
{
	const float32 speed = .5f;
	position += camera.GetRight() * (float)mod * speed;
	cameraLookAt += camera.GetRight() * (float)mod * speed;
}

void GodCamera::MoveCameraUp(int32 mod)
{
	const float32 speed = .5f;
	position += camera.GetUp() * (float)mod * speed;
	cameraLookAt += camera.GetUp() * (float)mod * speed;
}

void GodCamera::CameraLookAtAdjust(float changeX, float changeY)
{
	if (changeX != 0 || changeY != 0)
	{
		const float speed = .5f;
		Quat quatX(ROT_AXIS_ANGLE, camera.GetRight(), -changeY * speed);
		Quat quatY(ROT_Y, -changeX * speed);

		Vector4 newLookAtDir = camera.GetForward() * quatX * quatY;
		newLookAtDir.Normalize();
		cameraLookAt = position - newLookAtDir;
		Vector4 up = camera.GetUp() * quatX * quatY;
		cameraUp = up;
	}
}
