
#include "TransformSystem.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/ComponentTypeDefinition.hpp"
#include "ECS/Entity.hpp"

using namespace Musa;

void TransformSystem::Tick(float tick)
{
	UNUSED(tick);
}

// ComponentSet TransformSystem::GetRequiredComponents() const
// {
// 	ComponentSet group;
// 	AddComponentsTo<TransformComponent>(group);
// 	return group;
// }

void TransformSystem::AssociateEntity(const Musa::Entity& /*entity*/)
{
// 	ComponentSet entityComponents = Musa::GetComponentDescriptionOf(entity);
// 	Assert(entityComponents.ContainsTypes(GetRequiredComponents()));
// 	TransformWork work = 
// 	{ 
// 		GetComponentFrom<TransformComponent>(entity) // Transform
// 	};
// 	Assert(work.transform != nullptr);
// 	componentWork.Add(work);
}
