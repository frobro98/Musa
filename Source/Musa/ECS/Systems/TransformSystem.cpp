
#include "TransformSystem.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/ComponentTypeDefinition.hpp"
#include "ECS/Entity.hpp"

using namespace Musa;

void TransformSystem::Tick(float tick)
{
	UNUSED(tick);
	for (auto& work : componentWork)
	{
		ProcessTransform(work);
	}
}

ComponentGroupDescription TransformSystem::GetRequiredComponents() const
{
	ComponentGroupDescription group;
	group.AddComponents<TransformComponent>();
	return group;
}

void TransformSystem::AssociateEntity(const Musa::Entity& entity)
{
	ComponentGroupDescription entityComponents = Musa::GetComponentDescriptionOf(entity);
	Assert(entityComponents.ContainsTypes(GetRequiredComponents()));
	TransformWork work = 
	{ 
		GetComponentFrom<TransformComponent>(entity) // Transform
	};
	Assert(work.transform != nullptr);
	componentWork.Add(work);
}

void TransformSystem::ProcessTransform(TransformWork& work)
{
	TransformComponent* transform = work.transform;
	if (transform->IsDirty())
	{
		Matrix translation(TRANS, transform->GetPosition());
		Matrix scale(SCALE, transform->GetScale());
		transform->SetLocalTransform(scale * transform->GetRotation() * translation);
		transform->SetWorldTransform(transform->GetLocalTransform() /** parentTransform*/);
		
		// Transform children...

		transform->ResetDirtyFlag();
	}
	else
	{
		transform->ResetUpdatedFlag();
	}
}
