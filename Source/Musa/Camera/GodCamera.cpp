// Copyright 2020, Nathan Blane

#include "Math/Quat.hpp"
#include "Math/MathFunctions.hpp"
#include "GodCamera.hpp"
#include "Camera.h"
#include "Input/Input.hpp"
#include "Engine/FrameData.hpp"
#include "Input/GameInput.hpp"
#include "Debugging/DebugOutput.hpp"

GodCamera::GodCamera(GameWorld& world, Camera& cam)
	: GameObject(world),
	camera(&cam)
{
	position = camera->GetPosition();
	cameraLookAt = camera->GetLookAt();
	cameraUp = camera->GetUp();
}

void GodCamera::Update(float tick)
{
	constexpr f32 speed = 5.f;
	constexpr f32 look = 5.f;
	moveSpeed = speed * tick;
	lookSpeed = look * tick;
	
	camera->SetOrientationAndPosition(cameraLookAt, position, cameraUp);

	// TODO - This should be done in the SetWorld method...
	worldTransform = Matrix4(TRANS, position);
}

void GodCamera::InputCallback(const FrameInputs& inputs)
{
	for (const auto& action : inputs.actions)
	{
		Debug::Printf("%s\n", *action->inputName);
	}

	f32 changeX = 0, changeY = 0;
	for (const auto& range : inputs.ranges)
	{
		if (Math::IsNonZero(range.rangeValue))
		{
// 			if (range.input->type == Input::Mouse_XAxis ||
// 				range.input->type == Input::Gamepad_RightStick_XAxis)
// 			{
// 				changeX += range.rangeValue;
// 			}
// 			if (range.input->type == Input::Mouse_YAxis ||
// 				range.input->type == Input::Gamepad_RightStick_YAxis)
// 			{
// 				changeY += range.rangeValue;
// 			}
// 			if (range.input->type == Input::Gamepad_LeftStick_XAxis)
// 			{
// 				if (range.rangeValue > 0.f)
// 				{
// 					MoveCameraRight();
// 				}
// 				else if (range.rangeValue < 0.f)
// 				{
// 					MoveCameraLeft();
// 				}
// 			}
// 			else if (range.input->type == Input::Gamepad_LeftStick_YAxis)
// 			{
// 				if (range.rangeValue > 0.f)
// 				{
// 					MoveCameraForward();
// 				}
// 				else if (range.rangeValue < 0.f)
// 				{
// 					MoveCameraBackward();
// 				}
// 			}
// 
// 			if (range.input->type == Input::Gamepad_LeftTrigger)
// 			{
// 				MoveCameraDown();
// 			}
// 			else if (range.input->type == Input::Gamepad_RightTrigger)
// 			{
// 				MoveCameraUp();
// 			}
		}
	}

	CameraLookAtAdjust(changeX, changeY);

	for (const auto& state : inputs.states)
	{
		if (state->type == Input::Key_W)
		{
			MoveCameraForward();
		}
		else if (state->type == Input::Key_S)
		{
			MoveCameraBackward();
		}
		else if (state->type == Input::Key_A)
		{
			MoveCameraLeft();
		}
		else if (state->type == Input::Key_D)
		{
			MoveCameraRight();
		}
		else if (state->type == Input::Key_E)
		{
			MoveCameraUp();
		}
		else if (state->type == Input::Key_Q)
		{
			MoveCameraDown();
		}
	}
}

void GodCamera::MoveCameraForward()
{
	MoveCameraAlongAxis(camera->GetForward(), true);
}

void GodCamera::MoveCameraBackward()
{
	MoveCameraAlongAxis(camera->GetForward(), false);
}

void GodCamera::MoveCameraLeft()
{
	MoveCameraAlongAxis(camera->GetRight(), false);
}

void GodCamera::MoveCameraRight()
{
	MoveCameraAlongAxis(camera->GetRight(), true);
}

void GodCamera::MoveCameraUp()
{
	MoveCameraAlongAxis(camera->GetUp(), true);
}

void GodCamera::MoveCameraDown()
{
	MoveCameraAlongAxis(camera->GetUp(), false);
}

void GodCamera::MoveCameraAlongAxis(const Vector4& axis, bool positive)
{
	const f32 dirMod = positive ? 1.f : -1.f;
	const f32 speedMod = dirMod * moveSpeed;
	position += axis * speedMod;
	cameraLookAt += axis * speedMod;
}

void GodCamera::CameraLookAtAdjust(float changeX, float changeY)
{
	const f32 tick = Frame::GetTickTimeSeconds();
	if (changeX != 0 || changeY != 0)
	{
		Quat quatX(ROT_AXIS_ANGLE, camera->GetRight(), -changeY * lookSpeed);
		Quat quatY(ROT_Y, -changeX * lookSpeed);

		Vector4 newLookAtDir = camera->GetForward() * quatX * quatY;
		newLookAtDir.Normalize();
		cameraLookAt = position + newLookAtDir;
		Vector4 up = camera->GetUp() * quatX * quatY;
		cameraUp = up;
	}
}
