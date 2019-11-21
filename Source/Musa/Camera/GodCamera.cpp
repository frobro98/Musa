#include "GodCamera.hpp"
#include "Camera.h"
#include "Input/Input.hpp"
#include "Engine/FrameData.hpp"
#include "Input/GameInput.hpp"
#include "DebugOutput.h"

GodCamera::GodCamera(Camera& cam)
	: camera(cam)
{
	position = camera.GetPosition();
	cameraLookAt = camera.GetLookAt();
	cameraUp = camera.GetUp();
}

void GodCamera::Update(float tick)
{
	constexpr float32 speed = 50.f;
	constexpr float32 look = 5.f;
	moveSpeed = speed * tick;
	lookSpeed = look * tick;
	
	camera.SetOrientationAndPosition(cameraLookAt, position, cameraUp);

	// TODO - This should be done in the SetWorld method...
	world = Matrix4(TRANS, position);
}

void GodCamera::InputCallback(const FrameInputs& inputs)
{
	for (const auto& action : inputs.actions)
	{
		Debug::Printf("%s\n", *action->inputName);
	}

	float32 changeX = 0, changeY = 0;
	for (const auto& range : inputs.ranges)
	{
		if (Math::IsNonZero(range.rangeValue))
		{
			if (range.input->type == Inputs::Mouse_XAxis ||
				range.input->type == Inputs::Gamepad_RightStick_XAxis)
			{
				changeX += range.rangeValue;
			}
			if (range.input->type == Inputs::Mouse_YAxis ||
				range.input->type == Inputs::Gamepad_RightStick_YAxis)
			{
				changeY += range.rangeValue;
			}
			if (range.input->type == Inputs::Gamepad_LeftStick_XAxis)
			{
				if (range.rangeValue > 0.f)
				{
					MoveCameraRight();
				}
				else if (range.rangeValue < 0.f)
				{
					MoveCameraLeft();
				}
			}
			else if (range.input->type == Inputs::Gamepad_LeftStick_YAxis)
			{
				if (range.rangeValue > 0.f)
				{
					MoveCameraForward();
				}
				else if (range.rangeValue < 0.f)
				{
					MoveCameraBackward();
				}
			}

			if (range.input->type == Inputs::Gamepad_LeftTrigger)
			{
				MoveCameraDown();
			}
			else if (range.input->type == Inputs::Gamepad_RightTrigger)
			{
				MoveCameraUp();
			}
		}
	}

	CameraLookAtAdjust(changeX, changeY);

	for (const auto& state : inputs.states)
	{
		if (state->type == Inputs::Key_W)
		{
			MoveCameraForward();
		}
		else if (state->type == Inputs::Key_S)
		{
			MoveCameraBackward();
		}
		else if (state->type == Inputs::Key_A)
		{
			MoveCameraLeft();
		}
		else if (state->type == Inputs::Key_D)
		{
			MoveCameraRight();
		}
		else if (state->type == Inputs::Key_E)
		{
			MoveCameraUp();
		}
		else if (state->type == Inputs::Key_Q)
		{
			MoveCameraDown();
		}
	}
}

void GodCamera::MoveCameraForward()
{
	MoveCameraAlongAxis(camera.GetForward(), false);
}

void GodCamera::MoveCameraBackward()
{
	MoveCameraAlongAxis(camera.GetForward(), true);
}

void GodCamera::MoveCameraLeft()
{
	MoveCameraAlongAxis(camera.GetRight(), false);
}

void GodCamera::MoveCameraRight()
{
	MoveCameraAlongAxis(camera.GetRight(), true);
}

void GodCamera::MoveCameraUp()
{
	MoveCameraAlongAxis(camera.GetUp(), true);
}

void GodCamera::MoveCameraDown()
{
	MoveCameraAlongAxis(camera.GetUp(), false);
}

void GodCamera::MoveCameraAlongAxis(const Vector4& axis, bool positive)
{
	const float32 dirMod = positive ? 1.f : -1.f;
	const float32 speedMod = dirMod * moveSpeed;
	position += axis * speedMod;
	cameraLookAt += axis * speedMod;
}

void GodCamera::CameraLookAtAdjust(float changeX, float changeY)
{
	//Debug::Printf("Mouse X: %f, Mouse Y: %f\n", changeX, -changeY);
	const float32 tick = Frame::GetTickTimeSeconds();
	if (changeX != 0 || changeY != 0)
	{
		Quat quatX(ROT_AXIS_ANGLE, camera.GetRight(), -changeY * lookSpeed);
		Quat quatY(ROT_Y, -changeX * lookSpeed);

		Vector4 newLookAtDir = camera.GetForward() * quatX * quatY;
		newLookAtDir.Normalize();
		cameraLookAt = position - newLookAtDir;
		Vector4 up = camera.GetUp() * quatX * quatY;
		cameraUp = up;
	}
}
