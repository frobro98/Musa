#pragma once

#include "PCSTree/PCSNode.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Texture/Color.hpp"

class ShaderProgram;

enum class KeyInput;
enum class KeyboardState;

class GameObject : public PCSNode<GameObject>
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

	void SetPos(const Vector& position);
	void SetPos(float x, float y, float z);
	void SetRotation(float angleX, float angleY, float angleZ);
	void SetScale(float scaleX, float scaleY, float scaleZ);

	virtual void InputSetup();
	virtual void MoveForward(int32 mod);
	virtual void MoveRight(int32 mod);
	virtual void MoveUp(int32 mod);

	CollisionInfo GetCollisionInfo() const;

	void SetDebugColor(Color color);

	void SetWorld(const Matrix& newWorld);
	Matrix GetWorld() const;
protected:
	virtual void TransformDebugVolume();

protected:
	// TODO - implement own version of unique_ptr
	Matrix world;

	Vector position;
	float32 rotX;
	float32 rotY;
	float32 rotZ;
	Vector scale;

	std::unique_ptr<Model> model;
	Model* debugVolume = nullptr;
	Scene* scene = nullptr;
	bool debugShown = false;

	bool shouldRender = true;
	bool pad[2] = { false, false };
};