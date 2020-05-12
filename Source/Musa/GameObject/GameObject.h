// Copyright 2020, Nathan Blane

#pragma once

#include "Types/UniquePtr.hpp"
#include "Math/Vector4.hpp"
#include "Math/Matrix4.hpp"
#include "Mesh/Mesh.h"
#include "Model/Model.h"
#include "Color.hpp"

class GameWorld;

enum class KeyInput;
enum class KeyboardState;

class GameObject
{
public:
	GameObject(GameWorld& gameWorld);
	virtual ~GameObject() = default;
	//GameObject(const GameObject& go);
	GameObject(GameObject&& go) noexcept;

	//GameObject& operator=(const GameObject& go);
	GameObject& operator=(GameObject&& go) noexcept;

	virtual void SetupInputs() {}

	void SetModel(Model* m);
	virtual void ShowDebugVolume(bool show);

	void ToggleDebugVolume();
	void ToggleDebugColor();

	virtual void Update(float tick);

	void SetPos(const Vector4& position);
	void SetPos(float x, float y, float z);
	void SetRotation(float angleX, float angleY, float angleZ);
	void SetScale(float scaleX, float scaleY, float scaleZ);

	inline Vector4 GetPosition() const { return position; }

	virtual void InputSetup();
	virtual void MoveForward(int32 mod);
	virtual void MoveRight(int32 mod);
	virtual void MoveUp(int32 mod);

	SphereBounds GetCollisionInfo() const;

	void SetDebugColor(Color32 color);

	void SetWorld(const Matrix4& newWorld);
	Matrix4 GetWorld() const;
protected:
	virtual void TransformDebugVolume();

protected:
	// TODO - implement own version of unique_ptr
	Matrix4 worldTransform;

	Vector4 position;
	float32 rotX;
	float32 rotY;
	float32 rotZ;
	Vector4 scale;

	GameWorld* world;
	UniquePtr<Model> model;
	Model* debugVolume = nullptr;
	
	bool debugShown = false;
	bool shouldRender = true;
};