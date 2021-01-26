// Copyright 2020, Nathan Blane

#include "GameInputSystem.hpp"
#include "ECS/Components/AnalogChangeEvent.hpp"
#include "ECS/Components/MouseMoveEventComponent.hpp"
#include "ECS/Components/ButtonEventComponent.hpp"
#include "ECS/Components/InputContextComponent.hpp"
#include "ECS/Components/GameInputComponent.hpp"
#include "ECS/World.hpp"

#include "Math/MathFunctions.hpp"
#include "Input/InputContext.hpp"
#include "Input/InputEvents.hpp"

using namespace Musa;

static forceinline f32 NormalizeValueToRange(f32 value, const InputRange& contextRange)
{
	const InputRange& range = contextRange;
	value = Math::Clamp(value, range.minRawRange, range.maxRawRange);

	f32 lerpT = (value - range.minRawRange) / (range.maxRawRange - range.minRawRange);
	return Math::Lerp(range.minNormalizedRange, range.maxNormalizedRange, lerpT);
}

// TODO - Not const correct...
static void ProcessButtonEvents(GameInputComponent& gameInput, DynamicArray<ChunkComponentAccessor>& buttonChunks, const InputContextComponent& context)
{
	// Transform button events into game commands when compared to the input context
	// Game command is the actual command that the input context is looking for and
	// contains the relevant information, like
	for (auto& chunk : buttonChunks)
	{
		ChunkArray<const ButtonEventComponent> buttonEvents = chunk.GetArrayOf<const ButtonEventComponent>();
		Assert(buttonEvents.IsValid());

		for (const auto& buttonEvent : buttonEvents)
		{
			Input::Buttons button = buttonEvent.event.button;
			Assert(button < Input::Buttons::KM_Max);

			Input::ButtonState state = buttonEvent.event.state;
			bool isPressed = state.endedDown && !state.previouslyDown;
			NOT_USED bool isReleased = !state.endedDown && state.previouslyDown;
			
			// TODO - Need to officially make the input context a specific file, in doing so, 
			// rework the data and how it's stored
			// Look up to see if input is actually in input context
			if (ContainsInputAsAction(button, *context.inputConext))
			{
				if (isPressed)
				{
					gameInput.inputActions.Add(InputAction{ button, InputEventType::Pressed });
				}
			}
			if (ContainsInputAsState(button, *context.inputConext))
			{
				auto& inputStates = gameInput.inputStates;
				i32 stateIndex = inputStates.FindFirstIndexUsing([&] (const InputState& s){
					return s.input == button;
				});

				if (stateIndex < 0)
				{
					// Should be pressed
					Assert(isPressed);
					inputStates.Add(InputState{ button, 1.f });
				}
				else
				{
					inputStates.Remove((u32)stateIndex);
				}

				// NOTE - There aren't any analog/mouse movement, so no need to check for ranges
// 				if (ContainsInputAsRange(button, *gameInput.inputConext))
// 				{
// 
// 				}
// 				else
// 				{
// 					// TODO - Rework states to link into ranges somehow. Mouse movement/Gamepad 
// 					// stick movement should be treated the same as WASD because they can be similar.
// 					// Ranges could be an optional addition to input contexts that dictate a range
// 				}
			}
		}
	}
}

// TODO - Not const correct...
static void ProcessMouseMoveEvents(GameInputComponent& gameInput, DynamicArray<ChunkComponentAccessor>& mouseMoveChunks, const InputContextComponent& context)
{
	for (auto& chunk : mouseMoveChunks)
	{
		ChunkArray<const MouseMoveEventComponent> mouseMoveEvents = chunk.GetArrayOf<const MouseMoveEventComponent>();
		Assert(mouseMoveEvents.IsValid());

		for (const auto& mouseMoveEvent : mouseMoveEvents)
		{
			IntVector2 mouseDelta = mouseMoveEvent.event.deltaPosition;
			
			// Mouse X movement
			if (RangedInput* rangedInput = GetInputRange(Input::Mouse_XAxis, *context.inputConext);
				rangedInput != nullptr)
			{
				f32 mouseXValue = NormalizeValueToRange((f32)mouseDelta.x, rangedInput->range);
				InputState* mouseXState = gameInput.inputStates.FindFirstUsing([&](const InputState& state) {
					return state.input == Input::Mouse_XAxis;
					});
				if (mouseXState != nullptr)
				{
					mouseXState->value += mouseXValue;
				}
				else
				{
					gameInput.inputStates.Add(InputState{ Input::Mouse_XAxis, mouseXValue });
				}
			}

			// Mouse Y movement
			if (RangedInput* rangedInput = GetInputRange(Input::Mouse_YAxis, *context.inputConext);
				rangedInput != nullptr)
			{
				f32 mouseYValue = NormalizeValueToRange((f32)mouseDelta.y, rangedInput->range);
				InputState* mouseYState = gameInput.inputStates.FindFirstUsing([&](const InputState& state) {
					return state.input == Input::Mouse_YAxis;
					});
				if (mouseYState != nullptr)
				{
					mouseYState->value += mouseYValue;
				}
				else
				{
					gameInput.inputStates.Add(InputState{ Input::Mouse_YAxis, mouseYValue });
				}
			}
		}
	}
}

// TODO - Not const correct...
static void ProcessAnalogChangeEvents(GameInputComponent& gameInput, DynamicArray<ChunkComponentAccessor>& analogChangeChunks, const InputContextComponent& context)
{
	for (auto& chunk : analogChangeChunks)
	{
		ChunkArray<const AnalogChangeEventComponent> analogChangeEvents = chunk.GetArrayOf<const AnalogChangeEventComponent>();
		Assert(analogChangeEvents.IsValid());

		for (const auto& analogChangeEvent : analogChangeEvents)
		{
			Input::Buttons analogInput = analogChangeEvent.event.analogButton;
			f32 analogValue = analogChangeEvent.event.normValue;

			if (ContainsInputAsState(analogInput, *context.inputConext))
			{
				InputState* analogState = gameInput.inputStates.FindFirstUsing([&](const InputState& state) {
					return state.input == analogInput;
					});
				if (analogState != nullptr)
				{
					analogState->value += analogValue;
				}
				else
				{
					gameInput.inputStates.Add(InputState{ analogInput, analogValue });
				}
			}
		}
	}
}

void GameInputSystem::Initialize()
{
	gameInputInitQuery = &GetQueryFor(
		DescribeQuery()
		.Require<InputContextComponent>()
		.None<GameInputComponent>()
	);

	existingGameInputQuery = &GetQueryFor(
		DescribeQuery()
		.Require<InputContextComponent, GameInputComponent>()
	);

	buttonEventQuery = &GetQueryFor(
		DescribeQuery()
		.Require<ButtonEventComponent>()
	);

	mouseMoveEventQuery = &GetQueryFor(
		DescribeQuery()
		.Require<MouseMoveEventComponent>()
	);

	analogEventQuery = &GetQueryFor(
		DescribeQuery()
		.Require<AnalogChangeEventComponent>()
	);
}

void GameInputSystem::Update()
{
	ProcessInputEvents();
	ClearInputEvents();
}

void GameInputSystem::ProcessInputEvents() const
{
	auto gameInputChunks = GetQueryChunks(*existingGameInputQuery);
	for (auto& gameInputChunk : gameInputChunks)
	{
		auto buttonChunks = GetQueryChunks(*buttonEventQuery);
		Assert(buttonChunks.Size() == 1);
		auto mouseMoveChunks = GetQueryChunks(*mouseMoveEventQuery);
		Assert(mouseMoveChunks.Size() == 1);
		auto analogEventChunks = GetQueryChunks(*analogEventQuery);
		Assert(analogEventChunks.Size() == 1);

		// Par
		ChunkArray<InputContextComponent> inputContexts = gameInputChunk.GetArrayOf<InputContextComponent>();
		Assert(inputContexts.IsValid());
		ChunkArray<GameInputComponent> gameInputs = gameInputChunk.GetArrayOf<GameInputComponent>();

		for (u32 i = 0; i < inputContexts.size; ++i)
		{
			InputContextComponent& inputContext = inputContexts[i];
			GameInputComponent& gameInput = gameInputs[i];

			ProcessButtonEvents(gameInput, buttonChunks, inputContext);

			ProcessMouseMoveEvents(gameInput, mouseMoveChunks, inputContext);

			ProcessAnalogChangeEvents(gameInput, analogEventChunks, inputContext);
		}
	}
}

void GameInputSystem::ClearInputEvents()
{
	{
		auto buttonEntities = GetQueriedEntities(*buttonEventQuery);
		for (const auto& entity : buttonEntities)
		{
			GetWorld().DestroyEntity(entity);
		}
	}

	{
		auto mouseMoveEntities = GetQueriedEntities(*mouseMoveEventQuery);
		for (const auto& entity : mouseMoveEntities)
		{
			GetWorld().DestroyEntity(entity);
		}
	}

	{
		auto analogEventEntities = GetQueriedEntities(*analogEventQuery);
		for (const auto& entity : analogEventEntities)
		{
			GetWorld().DestroyEntity(entity);
		}
	}
}
