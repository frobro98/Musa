#pragma once

#include "Math/Vector4.h"
#include "Math/Matrix4.h"
#include "Mesh/Mesh.h"
#include "Model/Model.h"

class ShaderProgram;

enum class KeyInput;
enum class KeyboardState;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject() = default;
	GameObject(const GameObject& go);
	GameObject(GameObject&& go) noexcept;

	GameObject& operator=(const GameObject& go);
	GameObject& operator=(GameObject&& go) noexcept;

	void AssociateScene(class Scene& scene);

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
	Matrix4 world;

	Vector4 position;
	float32 rotX;
	float32 rotY;
	float32 rotZ;
	Vector4 scale;

	std::unique_ptr<Model> model;
	Model* debugVolume = nullptr;
	Scene* scene = nullptr;
	bool debugShown = false;

	bool shouldRender = true;
	bool pad[2] = { false, false };
};