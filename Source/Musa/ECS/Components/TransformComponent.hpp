#pragma once

#include "ECS/Component.hpp"
#include "Math/Vector.h"
#include "Math/Quat.h"
#include "Math/Matrix.h"

struct TransformComponent : public Musa::Component
{
	DECLARE_COMPONENT(TransformComponent);

	TransformComponent();

	void SetPosition(const Vector& pos);
	void SetRotation(const Quat& rot);
	void SetScale(const Vector& sca);

	Vector GetPosition() const;
	Quat GetRotation() const;
	Vector GetScale() const;

	void SetWorldTransform(const Matrix& mat);
	void SetLocalTransform(const Matrix& mat);
	Matrix GetWorldTransform() const;
	Matrix GetLocalTransform() const;

	bool IsDirty() const;
	bool WasUpdatedThisFrame() const;
	void ResetDirtyFlag();
	void ResetUpdatedFlag();

private:
	void TransformChanged();

private:
	Matrix worldTransform;
	Matrix localTransform;
	Vector position;
	Quat   rotation;
	Vector scale;

	bool transformDirty = true;
	bool wasUpdated = false;
	bool pad[6] = { false, false, false, false, false, false };
};

