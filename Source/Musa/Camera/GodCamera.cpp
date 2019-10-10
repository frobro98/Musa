#include "GodCamera.hpp"
#include "Camera.h"
#include "Input/Input.hpp"
#include "Engine/FrameData.hpp"

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
	const float32 tick = Frame::GetTickTimeSeconds();
	const float32 speed = 25.f;
	const float32 speedMod = (float32)-mod * speed * tick;
	position += camera.GetForward() * speedMod;
	cameraLookAt += camera.GetForward() * speedMod;
}

void GodCamera::MoveCameraRight(int32 mod)
{
	const float32 tick = Frame::GetTickTimeSeconds();
	const float32 speed = 25.f;
	const float32 speedMod = (float32)mod * speed * tick;
	position += camera.GetRight() * speedMod;
	cameraLookAt += camera.GetRight() * speedMod;
}

void GodCamera::MoveCameraUp(int32 mod)
{
	const float32 tick = Frame::GetTickTimeSeconds();
	const float32 speed = 25.f;
	const float32 speedMod = (float32)mod * speed * tick;
	position += camera.GetUp() * speedMod;
	cameraLookAt += camera.GetUp() * speedMod;
}

void GodCamera::CameraLookAtAdjust(float changeX, float changeY)
{
	const float32 tick = Frame::GetTickTimeSeconds();
	if (changeX != 0 || changeY != 0)
	{
		const float32 speed = 25.f;
		Quat quatX(ROT_AXIS_ANGLE, camera.GetRight(), -changeY * speed * tick);
		Quat quatY(ROT_Y, -changeX * speed * tick);

		Vector4 newLookAtDir = camera.GetForward() * quatX * quatY;
		newLookAtDir.Normalize();
		cameraLookAt = position - newLookAtDir;
		Vector4 up = camera.GetUp() * quatX * quatY;
		cameraUp = up;
	}
}
