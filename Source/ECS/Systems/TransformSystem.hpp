#pragma once

#include "ECS/System.hpp"
#include "Containers/Array.h"

class TransformSystem : public Musa::System
{
public:
	virtual void Tick(float tick) override;
	virtual Musa::ComponentGroupDescription GetRequiredComponents() const override;
	virtual void AssociateEntity(const Musa::Entity& entity) override;

private:
	struct TransformWork
	{
		struct TransformComponent* transform;
	};

	void ProcessTransform(TransformWork& work);

private:
	Array<TransformWork> componentWork;
};

DEFINE_SYSTEM(TransformSystem);
