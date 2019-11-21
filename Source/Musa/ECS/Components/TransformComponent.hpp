#pragma once

#include "ECS/Component.hpp"
#include "Math/Vector4.hpp"
#include "Math/Quat.hpp"
#include "Math/Matrix4.hpp"

struct TransformComponent : public Musa::Component
{
	DECLARE_COMPONENT(TransformComponent);

	TransformComponent();

	void SetPosition(const Vector4& pos);
	void SetRotation(const Quat& rot);
	void SetScale(const Vector4& sca);

	Vector4 GetPosition() const;
	Quat GetRotation() const;
	Vector4 GetScale() const;

	void SetWorldTransform(const Matrix4& mat);
	void SetLocalTransform(const Matrix4& mat);
	Matrix4 GetWorldTransform() const;
	Matrix4 GetLocalTransform() const;

	bool IsDirty() const;
	bool WasUpdatedThisFrame() const;
	void ResetDirtyFlag();
	void ResetUpdatedFlag();

private:
	void TransformChanged();

private:
	Matrix4 worldTransform;
	Matrix4 localTransform;
	Vector4 position;
	Quat   rotation;
	Vector4 scale;

	bool transformDirty = true;
	bool wasUpdated = false;
	bool pad[6] = { false, false, false, false, false, false };
};

