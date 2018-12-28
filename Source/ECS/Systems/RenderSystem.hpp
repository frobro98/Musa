#pragma once

#include "ECS/System.hpp"
#include "Containers/Array.h"

class RenderSystem : public Musa::System
{
public:

	virtual void Tick(float deltaTime) override;
	virtual void OnDestruction() override;
	virtual Musa::ComponentGroupDescription GetRequiredComponents() const override;
	virtual void AssociateEntity(const Musa::Entity& entity) override;

private:
	struct RenderWork
	{
		const struct TransformComponent* transform;
		struct RenderComponent* render;
	};

private:
	Array<RenderWork> componentWork;
};
