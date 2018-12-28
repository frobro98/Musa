
#include "TransformComponent.hpp"

using namespace Musa;

DEFINE_COMPONENT(TransformComponent);

TransformComponent::TransformComponent()
	: Component(GetComponentType())
{
}

void TransformComponent::SetPosition(const Vector& pos)
{
	position = pos;
	TransformChanged();
}

void TransformComponent::SetRotation(const Quat& rot)
{
	rotation = rot;
	TransformChanged();
}

void TransformComponent::SetScale(const Vector& sca)
{
	scale = sca;
	TransformChanged();
}

Vector TransformComponent::GetPosition() const
{
	return position;
}

Quat TransformComponent::GetRotation() const
{
	return rotation;
}

Vector TransformComponent::GetScale() const
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
