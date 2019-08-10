
#include "TransformComponent.hpp"

using namespace Musa;

DEFINE_COMPONENT(TransformComponent);

TransformComponent::TransformComponent()
	: Component(GetComponentType())
{
}

void TransformComponent::SetPosition(const Vector4& pos)
{
	position = pos;
	TransformChanged();
}

void TransformComponent::SetRotation(const Quat& rot)
{
	rotation = rot;
	TransformChanged();
}

void TransformComponent::SetScale(const Vector4& sca)
{
	scale = sca;
	TransformChanged();
}

Vector4 TransformComponent::GetPosition() const
{
	return position;
}

Quat TransformComponent::GetRotation() const
{
	return rotation;
}

Vector4 TransformComponent::GetScale() const
{
	return scale;
}

void TransformComponent::SetWorldTransform(const Matrix& mat)
{
	worldTransform = mat;
	TransformChanged();
}

void TransformComponent::SetLocalTransform(const Matrix& mat)
{
	localTransform = mat;
	TransformChanged();
}

Matrix TransformComponent::GetWorldTransform() const
{
	return worldTransform;
}

Matrix TransformComponent::GetLocalTransform() const
{
	return localTransform;
}

bool TransformComponent::IsDirty() const
{
	return transformDirty;
}

bool TransformComponent::WasUpdatedThisFrame() const
{
	return wasUpdated;
}

void TransformComponent::ResetDirtyFlag()
{
	transformDirty = false;
}

void TransformComponent::ResetUpdatedFlag()
{
	wasUpdated = false;
}

void TransformComponent::TransformChanged()
{
	transformDirty = true;
	wasUpdated = true;
}
