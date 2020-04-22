#pragma once

#include "ECS/System.hpp"
#include "Containers/DynamicArray.hpp"

namespace Musa
{
struct Entity;
}

class CameraSystem : public Musa::System
{
public:

	virtual void Tick(float tick) override;
//	virtual Musa::ComponentSet GetRequiredComponents() const override;
	virtual void AssociateEntity(const Musa::Entity& entity) override;

private:
	struct CameraWork
	{
		const struct TransformComponent* transform;
		struct CameraComponent* camera;
	};

private:
	DynamicArray<CameraWork> componentWork;
};

DEFINE_SYSTEM(CameraSystem);