#pragma once

#include "ECS/System.hpp"
#include "Containers/DynamicArray.hpp"

class RenderSystem : public Musa::System
{
public:

	virtual void Tick(float deltaTime) override;
	//virtual void OnDestruction() override;
//	virtual Musa::ComponentSet GetRequiredComponents() const override;
	virtual void AssociateEntity(const Musa::Entity& entity) override;

	// GetDrawList() ???? Maybe?

private:
	struct RenderWork
	{
		const struct TransformComponent* transform;
		struct RenderComponent* render;
	};

	void ProcessRenderWork(RenderWork& work);

private:
	DynamicArray<RenderWork> componentWork;
};
